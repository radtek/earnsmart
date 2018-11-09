/* 
 * File:   ReturnCodes.h
 * Author: santony
 *
 * Created on August 6, 2012, 7:26 AM
 */

#ifndef RETURNCODES_H
#define	RETURNCODES_H

// following are standard codes. They are defined same in all libraries.
#define S_Success                   0x00000000  // success code is same in every library
#define S_Optional                  0x00000001
#define S_False                     0x00000002  // this means, no errors with the method, but there is nothing to return.
#define S_ThreadWait                0x00000003  // this means, no errors with the method, but there is nothing to return.
#define S_DataExists                0x00000004  // Returned when a method returns some data which already existed when the user intended to create that data.

#define E_UnknownError              0x80000001  // 
#define E_MemoryUnavailable         0x80000002
#define E_InvalidPointer            0x80000003
#define E_InvalidArg                0x80000004
#define E_NotImpl                   0x80000005
#define E_Thread                    0x80000006  // any multi-threading exception.
#define E_FileNotFound              0x80000007  // any file error
#define E_NotFound                  0x80000008  // any file error
#define E_NotDeserializable         0x80000009  // does implement IDeserializable
#define E_NotJsonDeserializable     0x8000000A
#define E_NotFoundRec_id            0x8000000B 
#define E_SecurityAuthorization     0x8000000C  // Security authorization failed.
#define E_ReadError                 0x8000000D  // a read error occured (file or device)
#define E_MutexLock                 0x8000000E  // a mutex lock 
#define E_NoProcessName             0x8000000F  // a processname is not set. use alternate method for this purpose
#define E_ConfigurationError        0x80000010  // a configuration file error.
#define E_NotSupported              0x80000011  // method call is not supported.
#define E_NoDataToReturn            0x80000012  // nothing to return.
#define E_HtmlParseError            0x80000013  // html parsing failed (libxml)
#define E_AlgorithmError            0x80000014  // regression tree or similar algo errors.
#define E_SystemError               0x80000015  // Some OS system error.  
#define E_IncorrectOperation        0x80000016
#define E_SessionUnknown            0x80000017  // Unknown session id.
#define E_NotAuthorized             0x80000018  // Not authorized to execute the function.  
#define E_ParentNotFound            0x80000019  // Parent record not found.  
#define E_SessionExpired            0x8000001A  // User's application session has expired. User may need to sign in again.
#define E_JsonParseError            0x8000001B  // An exception is through while parsing a json string. Verify if the json string is in valid form.

#define E_JsonRpcInsufficientParameters 0x8000001C  // Json-rpc communication error. Not enough parameters or incorrect parameters to call the method successfully. Check method documentation.
#define E_JsonRpcInvalidResponse    0x8000001D  // Json-rpc communication error. The response received from the method call is not valid or not properly constructed.
#define E_JsonRpcIncorrectMethod    0x8000001E  // Json-rpc communication error. Method Id is incorrect. Check 'id' value in the posted data and ensure that, that Id is valid for the server api.
#define E_JsonRpcMethodNotFound     0x8000001F  // Json-rpc communication error. Method given in the 'method' attribute is not recogonized by the server.
#define E_EmailSendError            0x80000020  // Failed to send email notification Please retry.

#define E_InsufficientParameters    0X80000021  // Not enough parameters provided.

//#define SUCCEEDED(x) ( ((SE_CODE)(x))  >= 0)

#define SUCCEEDED(x) (x & 0x80000000) == 0x00000000

#define ISOK(x) (x == S_Success)
#define FAILED(x) (x & 0x80000000) == 0x80000000
#define RETFAILED(x) if (FAILED(x)) return x

#define S_OK                        S_Success

#endif	/* RETURNCODES_H */

