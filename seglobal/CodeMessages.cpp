
#include "IObject.h"
#include <map>

using namespace std;

namespace se
{
    IErrorDescription::CodeMessages IErrorDescription::msgs = 
    {
        CodeMessageItem(S_Success, "Function call is success"),
        CodeMessageItem(S_Optional, "Return value of this function is optional and can be ignored."),
        CodeMessageItem(S_False, "Function returned successfully, but with a false condition which may be ignored such as no items foudn to add to a collection."),
        CodeMessageItem(S_ThreadWait, "A parallel method call is waiting in a thread"),
        CodeMessageItem(S_DataExists, "The item which was attempted to add to a collection already exists in the collection."),
        CodeMessageItem(E_UnknownError, "The reason for this error is unknown"),
        CodeMessageItem(E_MemoryUnavailable, "No memory available."),
        CodeMessageItem(E_InvalidPointer, "Invalid pointer passed as an out parameter."),
        CodeMessageItem(E_InvalidArg, "Provided argument is incomplete or incorrectly formatted."),
        CodeMessageItem(E_InsufficientParameters, "Parameters provided to execute this instruction are insufficient"),
        CodeMessageItem(E_NotImpl, "This method is not implemented"),
        CodeMessageItem(E_Thread, "A concurrent thread exception."),
        CodeMessageItem(E_FileNotFound, "The file is not found."),
        CodeMessageItem(E_NotFound, "Not found."),
        CodeMessageItem(E_NotDeserializable, "Not deserializable"),
        CodeMessageItem(E_NotJsonDeserializable, "Not Json deserializable"),
        CodeMessageItem(E_NotFoundRec_id, "Record id not found."),
        CodeMessageItem(E_SecurityAuthorization, "Security authorization failed."),
        CodeMessageItem(E_ReadError, "A read error such as from a file or table."),
        CodeMessageItem(E_MutexLock, "A mutex lock error."),
        CodeMessageItem(E_NoProcessName, "Process name is not defined."),
        CodeMessageItem(E_ConfigurationError, "There is a configuration error in the configuration file."),
        CodeMessageItem(E_NotSupported, "This call is not supported."),
        CodeMessageItem(E_NoDataToReturn, "No data to return."),
        CodeMessageItem(E_HtmlParseError, "HTML Parsing error."),
        CodeMessageItem(E_AlgorithmError, "Algorithm error."),
        CodeMessageItem(E_SystemError, "System error."),
        CodeMessageItem(E_IncorrectOperation, "Incorrect operation."),
        CodeMessageItem(E_SessionUnknown, "Unknown session identifier"),
        CodeMessageItem(E_SessionExpired, "User's application session has expired. User may need to sign in again."),
        CodeMessageItem(E_NotAuthorized, "Not authorized to execute the function."),
        CodeMessageItem(E_ParentNotFound, "Parent record not found"),
        CodeMessageItem(E_JsonParseError, "An exception is thrown while parsing a json string. Verify if the json string is in valid form."),
        CodeMessageItem(E_JsonRpcInsufficientParameters, "Json-rpc communication error. Not enough parameters to call the method successfully. Check method documentation."),
        CodeMessageItem(E_JsonRpcInvalidResponse, "Json-rpc communication error. The response received from the method call is not valid or not properly constructed."),
        CodeMessageItem(E_JsonRpcIncorrectMethod, "Json-rpc communication error. Method Id is incorrect. Check 'id' value in the posted data and ensure that, that Id is valid for the server api."),
        CodeMessageItem(E_JsonRpcMethodNotFound, "Json-rpc communication error. Method given in the 'method' attribute is not recogonized by the server."),
        CodeMessageItem(E_EmailSendError, "Failed to send email notification. Please retry.")
    };
    

    CSTR IErrorDescription::get_CodeMessage(SE_CODE errorCode)
    {
        auto find = msgs.find(errorCode);
        if (find == msgs.end())
            return "";
        else
            return find->second;
        
    }
    
    void GBLEXP IErrorDescription::__loadMessages__(const std::initializer_list<CodeMessageItem>& list)
    {
        for (auto m : list)
            if (msgs.end() == msgs.find(m.first))
                msgs.insert(m);
    }
    
}