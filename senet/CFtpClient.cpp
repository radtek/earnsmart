/* 
 * File:   CFtpClient.cpp
 * Author: santony
 * 
 * Created on November 1, 2012, 1:33 AM
 */

#include "senetinternal.h"
#include "CFtpClient.h"
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/regex.hpp>
#include "../seglobal/CDirectory.h"

//using namespace log4cxx;

namespace se
{
    namespace net
    {
        
        //static log4cxx::LoggerPtr _logger = Logger::getLogger("se.net.ftpclient");
        
        struct FtpFile
        {
            CSTR fileName ;
            FILE* outputFile;
        };
        
        struct callback_data 
        {
            CSTR targetdir;
            FILE *output;
            CSTR pattern;
            boost::regex rg;
            std::function<void(CSTR)> func;
        };        
        
        
        struct statedata
        {
            void* state;
        };
        

        static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);

        static long file_is_comming(struct curl_fileinfo *finfo,
                                    struct callback_data *data,
                                    int remains);

        static long file_is_downloaded(struct callback_data *data);

        static size_t write_it(char *buff, size_t size, size_t nmemb,
                               void *cb_data);
        
        static size_t browsedirectory( char *ptr, size_t size, size_t nmemb, void *userdata);
        
        CFtpClient::CFtpClient() : pCurl(0)
        {
            
        }

        CFtpClient::CFtpClient(const CFtpClient& orig) 
        {

        }

        CFtpClient::~CFtpClient() 
        {
            Close();
        }
        
        ErrorId CFtpClient::Open(const char* url)
        {
            if (!pCurl)
            {
                curl_global_init(CURL_GLOBAL_DEFAULT);
                pCurl = curl_easy_init();
                
                if (!pCurl)
                {
                    //LOG4CXX_ERROR(_logger, "Error initializing curl library.");

                    curl_global_cleanup();
                    
                    return E_FtpError;
                }
                
                
                curl_easy_setopt(pCurl, CURLOPT_URL, url);
                

                //LOG4CXX_INFO(_logger, "Successfully opened ftp client.")
                
                return S_Success;
            }
            
            

            //LOG4CXX_INFO(_logger, "Ftp client is already open. ");
            
            return S_FtpHostAlreadyConnected;
        }
        
        ErrorId CFtpClient::Close()
        {
            if (pCurl) // don't use it in a multi-thread environment. not supported.
            {
                curl_easy_cleanup(pCurl);
                
                curl_global_cleanup();
                
                pCurl = NULL;
                
                //LOG4CXX_INFO(_logger, "Successfully closed ftp client.")
                
                return S_Success;
            }
            return E_FtpAlreadyClosed;
        }
        
        ErrorId CFtpClient::BrowseRemoteFolders(CSTR url, std::function<void (CSTR) > callback)
        {
            ErrorId err = Open(url);
            if (err != S_Success)
                return err;
            
            statedata * pState = new statedata;
            pState->state = &callback;

            curl_slist * cmdlist = nullptr;
            curl_slist_append(cmdlist, "dir .");
            curl_easy_setopt(pCurl, CURLOPT_QUOTE, cmdlist);
            curl_easy_setopt (pCurl, CURLOPT_WILDCARDMATCH, 1L);
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, browsedirectory);
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, pState);
            curl_easy_setopt(pCurl, CURLOPT_URL, url);
            auto res = curl_easy_perform(pCurl);
            curl_easy_setopt(pCurl, CURLOPT_QUOTE, nullptr);
            curl_slist_free_all(cmdlist);
            delete pState;
            if (CURLE_OK != res)
            {
                //LOG4CXX_ERROR(_logger, "Curl reported file download as " << res);
                err =  E_FtpDownload ;
            }
            else
                err = S_Success;

            Close();
            return err;
        }
        
        
        ErrorId CFtpClient::GetFile(CSTR url, CSTR destination)
        {
            if (!destination)
                return E_FtpDestination;          
            
            ErrorId err = Open(url);
            if (err != S_Success)
                return err;
            
            /* make sure we can create the destination file 
             such as file directory is available. */
            
            string destFile = destination;
            string outDir = destFile.substr(0,  
                    destFile.find_last_of('/', destFile.length() - 1)
                    );
            
            // ensure directory exists, if not create one.
            if (!CDirectory::Exists(outDir.c_str()))
            {
                CDirectory::Create(outDir.c_str());
            }
            
            
            FtpFile fileInfo = { destination, 0 };
            
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, my_fwrite);
            
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &fileInfo );
            
            curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
            
            CURLcode res = curl_easy_perform(pCurl);
            
            if (fileInfo.outputFile)
                fclose(fileInfo.outputFile);
            
            err = S_Success;
            
            if (CURLE_OK != res)
            {
                //LOG4CXX_ERROR(_logger, "Curl reported file download as " << res);
                err =  E_FtpDownload ;
            }
            
            Close();
            
            return err;
        }
        

        ErrorId  CFtpClient::GetFiles(CSTR url, CSTR destination, CSTR filterPattern, std::function<void(CSTR)> cb)
        {
            if (!url)
                return E_InvalidArg;

            if (!destination)
                return E_FtpDestination;
            
            ErrorId err = Open(url);
            if (err != S_Success)
                return err;
            
            string outfilename = destination;
            if (outfilename.substr(outfilename.length()-1, 1)!="/")
                outfilename.append("/");
            
            // ensure directory exists, if not create one.
            if (!CDirectory::Exists(outfilename.c_str()))
            {
                CDirectory::Create(outfilename.c_str());
            }
            
            // check pattern
            // check if it is a wild card or regex
            // look for *
            // expansive if statement below for clarity.
            callback_data data = { outfilename.c_str(), 0, 0};
            data.func = cb;
            string strPattern = "";
            if (filterPattern)
            {
                strPattern = filterPattern;
                if (!strPattern.empty())
                {
                    bool bIsWildCard = false;
                    STR patternstr = (STR)strPattern.c_str();
                    STR find = strchr(patternstr, '*');
                    if (find != NULL)
                    {
                        // look for [ or ( or \ or ^
                        if (find = strchr(patternstr, '['))
                        {
                            bIsWildCard = false;
                        }
                        else if (find = strchr(patternstr, '('))
                        {
                            bIsWildCard = false;
                        }
                        else if (find = strchr(patternstr, '\\'))
                        {
                            bIsWildCard = false;
                        }
                        else if (find = strchr(patternstr, '^'))
                        {
                            bIsWildCard = false;
                        }
                        else
                        {
                            bIsWildCard = true;
                        }
                    }
                    // if filterPattern is a wildcard, convert that to a regex by replace * with
                    // [\w\W]*. Example if filter pattern is tes*.txt, replace * like this tes[\w\W]*.txt
                    // also . (full stop) should be replaced as \.

                    if (bIsWildCard)
                    {
                        using namespace boost;
                        replace_all(strPattern, "*", "[\\w\\W]*");
                        replace_all(strPattern, ".", "\\.");
                    }

                    data.pattern = strPattern.c_str();
                    data.rg = strPattern;
                }
            }
            
            
            curl_easy_setopt (pCurl, CURLOPT_WILDCARDMATCH, 1L);
            
            curl_easy_setopt (pCurl, CURLOPT_CHUNK_BGN_FUNCTION, file_is_comming);
            
            curl_easy_setopt (pCurl, CURLOPT_CHUNK_END_FUNCTION, file_is_downloaded);
            
            curl_easy_setopt (pCurl, CURLOPT_WRITEFUNCTION, write_it) ;
            
            curl_easy_setopt (pCurl, CURLOPT_CHUNK_DATA, &data);
            
            curl_easy_setopt (pCurl, CURLOPT_WRITEDATA, &data );
            
            curl_easy_setopt (pCurl, CURLOPT_URL, url );

            CURLcode res = curl_easy_perform(pCurl);

            err = S_Success;
            
            if (CURLE_OK != res)
            {
                //LOG4CXX_ERROR(_logger, "Curl reported file download as " << res);
                err =  E_FtpDownload ;
            }
            
            Close();
            
            return err;
        }
        
        static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
        {
          struct FtpFile *out=(struct FtpFile *)stream;
          if(out && !out->outputFile) 
          {
            /* open file for writing */ 
            out->outputFile=fopen(out->fileName, "wb");
            if(!out->outputFile)
              return -1; /* failure, can't open file to write */ 
          }
          return fwrite(buffer, size, nmemb, out->outputFile);
        }
        
        static long file_is_comming(struct curl_fileinfo *finfo,
                                    struct callback_data *data,
                                    int remains)
        {
            //"%3d %40s %10luB ", 
          //LOG4CXX_INFO(_logger, remains << finfo->filename << (unsigned long)finfo->size);

          switch(finfo->filetype) 
          {
          case CURLFILETYPE_DIRECTORY:
            //LOG4CXX_INFO(_logger, " DIR.");
            break;
          case CURLFILETYPE_FILE:
            //LOG4CXX_INFO(_logger, " FILE.");
            break;
          default:
            //LOG4CXX_INFO(_logger, " OTHER.");
            break;
          }

          if(finfo->filetype == CURLFILETYPE_FILE) 
          {
            /* do not transfer files >= 50B */ 
//            if(finfo->size > 50) {
//              printf("SKIPPED\n");
//              return CURL_CHUNK_BGN_FUNC_SKIP;
//            }
              
              if (data->pattern)
              {

                // match regex
                boost::cmatch match;
                if (!boost::regex_match(finfo->filename, match, data->rg))
                {
                    return CURL_CHUNK_BGN_FUNC_SKIP;
                }
                
              }
              
              string outfilename = data->targetdir;
              outfilename.append(finfo->filename);

              data->output = fopen(outfilename.c_str(), "w");
              if(!data->output) 
              {
                  return CURL_CHUNK_BGN_FUNC_FAIL;
              }
              
              if (data->func)
                data->func(outfilename.c_str());
          }

          return CURL_CHUNK_BGN_FUNC_OK;
        }

        static long file_is_downloaded(struct callback_data *data)
        {
          if(data->output) 
          {
            //LOG4CXX_INFO(_logger, "DOWNLOADED.");
            fclose(data->output);
            data->output = 0x0;
          }
          return CURL_CHUNK_END_FUNC_OK;
        }

        static size_t write_it(char *buff, size_t size, size_t nmemb,
                               void *cb_data)
        {
          callback_data* data = static_cast<callback_data*>(cb_data);
          size_t written = 0;
          if(data->output)
            written = fwrite(buff, size, nmemb, data->output);
          else
            /* listing output */ 
            written = fwrite(buff, size, nmemb, stdout);
          return written;
        }        
        
        static size_t browsedirectory( char *ptr, size_t size, size_t nmemb, void *userdata)
        {
            statedata* pState = static_cast<statedata*>(userdata);
            auto func = static_cast<std::function< void(CSTR )>*>(pState->state);
            (*func)((CSTR)ptr);
            return 0;
        }
        

    }
    
}

