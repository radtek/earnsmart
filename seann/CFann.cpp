/* 
 * File:   CFann.cpp
 * Author: santony
 * 
 * Created on August 2, 2015, 8:57 AM
 */

#include "CFann.h"
#include "../seglobal/exceptions.h"
#include "fann.h"
#include "../seglobal/globallogger.h"
using namespace se;
using namespace se::math;

CFann::CFann() 
{
}

CFann::CFann(const std::string& annName) : _annName(annName)
{
    Load();
}

CFann::CFann(const CFann& orig) 
{
    _annName = orig._annName;
    _pData = orig._pData;
    _fanndata = orig._fanndata; // note this is only a pointer copy.
    _ann = orig._ann;
    _bTrained = orig._bTrained;
}

RP<IExTrainingTable> CFann::get_TrainingData()
{
    return _pData;
}

CFann& CFann::set_TrainingData(IExTrainingTable* data)
{
    if (!data)
    {
        BERR << "data argument is null.";
        throw seexception(E_InvalidArg, "Training data is invalid.");
    }
    _pData = data;
    ConstructTrainingData();
    return *this;
}

void CFann::ConstructTrainingData()
{
    if (_fanndata)
    {
        fann_destroy_train(_fanndata);
        _fanndata = nullptr;
    }
    
    BDBG << "Training data has " << get_NumberOfInputs() << " input neurons, " << get_NumberOfOutputs() << " output neurons and " << get_TrainingRecordCount() << " training pairs.";
    BDBG << "Attempting to initialize training data set";
    _fanndata = fann_create_train(get_TrainingRecordCount(), get_NumberOfInputs(), get_NumberOfOutputs());
    if (_fanndata == nullptr)
    {
        BERR << "fann_create_train call failed to construct internal training data.";
        throw seexception(E_TrainingDataSetNotFound, "Failed to create training data");
    }
    BDBG << "After initialization, now loading data.";
    int i=0;
    for (IExTrainingRecord* pRec : *_pData)
    {
        for (int j = 0; i<_pData->get_AttributeFieldsCount(); ++j)
        {
            _fanndata->input[i][j] = pRec->get_Value(j);
        }
        _fanndata->output[i][0] = pRec->get_Result();
        i++;
    }
}


unsigned int CFann::get_NumberOfInputs()
{
    if (!_pData)
        return 0;
    return _pData->get_AttributeFieldsCount();
}

unsigned int CFann::get_NumberOfOutputs()
{
    if (!_pData)
        return 0;
    return 1;
}

unsigned int CFann::get_TrainingRecordCount()
{
    if (!_pData)
        return 0;
    return _pData->get_Count();
}

CFann&  CFann::CreateBasicNetwork(U32 numberOfNeuronsInHidden)
{
    U32 total = get_NumberOfInputs() + get_NumberOfOutputs();
    if (numberOfNeuronsInHidden < total )
    {
        BWRN << "The number of hidden neurons must be at the least the sum of number of inputs and number of outputs. Automatically set to the default value.";
        numberOfNeuronsInHidden = total;
    }
    HiddenNeurons n = { numberOfNeuronsInHidden };
    return CreateStandardNetwork(n);
}


CFann& CFann::CreateStandardNetwork(const HiddenNeurons& numberOfHiddenNeurons)
{
    // allocate a layer 
    auto layerSize = 2 + numberOfHiddenNeurons.size();
    U32* layers = (U32*)calloc( layerSize, sizeof(U32));
    if (!layers)
    {
        throw seexception(E_MemoryUnavailable, "Memory error. Cannot allocate memory for layer array.");
    }
    int i=1;

    layers[0] = get_NumberOfInputs();
    for (const U32& h : numberOfHiddenNeurons)
        layers[i++] = h;
    layers[i] = get_NumberOfOutputs();
    
    _ann = fann_create_standard_array(layerSize, layers);
    
    free(layers);
    
    BDBG << "Successfully initialized ANN network";
    
    return *this;
}

CFann& CFann::CreateSparseNetwork(R8 connectionRate, const HiddenNeurons& numberOfHiddenNeurons)
{
    // allocate a layer 
    auto layerSize = 2 + numberOfHiddenNeurons.size();
    U32* layers = (U32*)calloc( layerSize, sizeof(U32));
    if (!layers)
    {
        throw seexception(E_MemoryUnavailable, "Memory error. Cannot allocate memory for layer array.");
    }
    int i=1;

    layers[0] = get_NumberOfInputs();
    for (const U32& h : numberOfHiddenNeurons)
        layers[i++] = h;
    layers[i] = get_NumberOfOutputs();
    
    _ann = fann_create_sparse_array(connectionRate, layerSize, layers);
    
    free(layers);
    
    BDBG << "Successfully initialized ANN network";

    return *this;
}

CFann& CFann::CreateShortcutNetwork(const HiddenNeurons& numberOfHiddenNeurons)
{
    // allocate a layer 
    auto layerSize = 2 + numberOfHiddenNeurons.size();
    U32* layers = (U32*)calloc( layerSize, sizeof(U32));
    if (!layers)
    {
        throw seexception(E_MemoryUnavailable, "Memory error. Cannot allocate memory for layer array.");
    }
    int i=1;

    layers[0] = get_NumberOfInputs();
    for (const U32& h : numberOfHiddenNeurons)
        layers[i++] = h;
    layers[i] = get_NumberOfOutputs();
    
    _ann = fann_create_shortcut_array(layerSize, layers);
    
    free(layers);
    
    BDBG << "Successfully initialized ANN network";

    return *this;
}

CFann& CFann::Train()
{
    TrainingSetup setup;
    return Train(setup);
}

CFann&  CFann::Train(const TrainingSetup& setup)
{
    if (_ann)
    {
        BERR << "ANN is not created. Use one of the 'Create' method.";
        throw seexception(E_IncorrectOperation, "ANN is not created. Use one of the 'Create' method.");
    }
    
    BDBG << "Starting to train data";
    fann_set_activation_function_hidden(_ann, setup.ActivationFunctionHidden);
    fann_set_activation_function_output(_ann, setup.ActivationFunctionOutput);
    fann_train_on_data(_ann, _fanndata, setup.MaximumEpochs, setup.EpochsBetweenReports, setup.DesiredError);
    _bTrained = true;
    Save();
    BDBG << "Successfully completed training data";
    return *this;
}



const OmniType& CFann::Run(IExTrainingRecord* expectedInputs)
{
    if (!_ann || !_bTrained)
    {
        BERR << "ANN is not trained. Invoke Train method.";
        throw seexception(E_IncorrectOperation, "ANN is not trained. Invoke Train method.");
    }
    
    
    if (!expectedInputs)
        THROWIFFAILED(_pData->get_FutureRecord(&expectedInputs), "No future record available.");

    fann_type* inputs = (fann_type*)calloc(get_NumberOfInputs(), sizeof(fann_type));
    
    for (auto i = 0; i<get_NumberOfInputs(); ++i)
    {
        inputs[i] = expectedInputs->get_Value(i);
    }
    expectedInputs->set_Result(fann_run(_ann, inputs));
    free(inputs);
    return expectedInputs->get_Result();
}

CFann& CFann::Load()
{
    if (!_annName.empty())
    {
        _ann = fann_create_from_file(_annName.c_str());
        if (!_ann)
            throw seexception("ANN data was not loaded.");
        _bTrained = true;
    }
    return *this;
}



CFann& CFann::Save()
{
    if (!_annName.empty() && _ann && !_bTrained)
    {
        fann_save(_ann, _annName.c_str());
    }
    else
    {
        throw seexception(E_IncorrectOperation, "ANN is not trained. Call Train method.");
    }
    return *this;
}


CFann::~CFann() 
{
    if (_fanndata)
    {
        fann_destroy_train(_fanndata);
        _fanndata = nullptr;
    }
    if (_ann)
    {
        fann_destroy(_ann);
        _ann = nullptr;
    }
}

