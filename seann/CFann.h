/* 
 * File:   CFann.h
 * Author: santony
 *
 * Created on August 2, 2015, 8:57 AM
 */

#ifndef CFANN_H
#define	CFANN_H

#include <string>
#include "../seglobaltypedefs.h"
#include "../setraining/setrainingapi.h"
#include "floatfann.h"
#include "fann_train.h"
#include <vector>


using namespace se::training;


namespace se
{
    namespace math
    {

        typedef std::vector<U32> HiddenNeurons;
        typedef fann_activationfunc_enum ActivationFunctions;
        struct TrainingSetup
        {
            U32 MaximumEpochs = 500000;
            U32 EpochsBetweenReports = 1000;
            R8 DesiredError = 0.001;
            ActivationFunctions ActivationFunctionHidden = FANN_SIGMOID_SYMMETRIC;
            ActivationFunctions ActivationFunctionOutput = FANN_SIGMOID_SYMMETRIC;
        };
        
        class CFann 
        {
        public:
            CFann();
            CFann(const std::string& annName);
            CFann(const CFann& orig);
            virtual ~CFann();
        
            U32 get_NumberOfInputs();
            U32 get_NumberOfOutputs();
            U32 get_TrainingRecordCount();
            
            RP<IExTrainingTable> get_TrainingData();
            
            CFann& set_TrainingData(IExTrainingTable* data);
            
            //Creates a standard fully connected backpropagation neural network.
            // Specify how many neurons in a single hidden network
            CFann& CreateBasicNetwork(U32 numberOfNeuronsInHidden);
            
            //Prepare a standard fully connected backpropagation neural network. 
            // Specify how many neurons in each hidden network
            CFann& CreateStandardNetwork(const HiddenNeurons& numberOfHiddenNeurons);
            
            //Creates a standard backpropagation neural network, which is not fully connected.
            //Specify connection rate (a value between 0 and 1) which tells us how many connections will be in the network.
            // If connection rate is set to 1, the network will be fully connected. If it is
            // set to .5 only half of the connections will be set. For a standard network,
            // connection rate is set to 1.
            //Specify how many neurons in each hidden network
            CFann& CreateSparseNetwork(R8 connectionRate, const HiddenNeurons& numberOfHiddenNeurons);
            
            //Creates a standard backpropagation neural network, which is not fully connected and which
            //also has shortcut connections.
            // Shortcut connections are connections that skip layers. A fully connected network with shortcut
            // connections is a network where all the neurons are connected to all neurons in later  layers including
            // direct connections from the input layer to output layer.
            //Specify how many neurons in each hidden network
            CFann& CreateShortcutNetwork(const HiddenNeurons& numberOfHiddenNeurons);
            
            // Training using default values.
            CFann& Train();
            
            CFann& Train(const TrainingSetup& setup);
            
            // Run the input through the neural network and return the expected output.
            // When expectedInputs is set to null, training table's Future record will be used.
            const OmniType& Run(IExTrainingRecord* expectedInputs = nullptr);

            CFann& Load();
            
            // Save the network into a file for future use. This method will work only if CFann(const std::string& annName) constructor with a name is used to create this instance.
            CFann& Save();
            
           
        private:
            void ConstructTrainingData();
            
        private:
            std::string _annName;
            SP<IExTrainingTable> _pData;
            struct fann_train_data* _fanndata = nullptr;
            struct fann* _ann = nullptr;
            bool _bTrained = false;
        };
        
    }
}


#endif	/* CFANN_H */

