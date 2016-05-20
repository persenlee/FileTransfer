//
//  DMFileUploadManager.hpp
//  FileTransfer
//
//  Created by duomai on 16/5/12.
//
//

#ifndef DMFileUploadManager_hpp
#define DMFileUploadManager_hpp

#include <iostream>
#include <openssl/sha.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/filestream.h>
#include "DMFile.hpp"
#include "DMUploadingRule.hpp"
using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

using namespace std;
class DMFileUploadHandler;

static const string  DM_UPLOAD_BASE_URL = "http://192.168.1.124:9000/";
static const int64_t DM_UPLOAD_REQUEST_TIMEOUT = 3 * 60; //second

typedef enum {
    DMUploadingStrategyDefault
} DMUploadingStrategy;  //上传策略

class DMFileUploadManager {
public:
    static DMFileUploadManager& defaultManager() {
        static DMFileUploadManager manager;
        return manager;
    }
    void uploadFile(DMFile const &file,DMFileUploadHandler *handler);
    void uploadFileWithFileURI(string const &fileURI,DMFileUploadHandler *handler);
 
    DMFileUploadHandler *uploadHandler;
    DMUploadingStrategy uploadStrategy;
    void setUploadURLStr(string urlStr);
    void setUserSessionToken(string token);
private:
    DMFileUploadManager() :_uploadURLStr(DM_UPLOAD_BASE_URL), _httpClient(U(_uploadURLStr))
    {
        http_client_config config;
        config.set_timeout(utility::seconds(DM_UPLOAD_REQUEST_TIMEOUT));
        _httpClient = http_client(U(_uploadURLStr),config);
    }
    DMFileUploadManager(DMFileUploadManager const&);
    DMFileUploadManager& operator=(DMFileUploadManager const &);
    string _uploadURLStr;
    http_client _httpClient;
//    DMUploadingRule _defaultRule;
//    DMFile _file;
    string _user_session_token;
};

//上传回调
class DMFileUploadHandler
{
public:
    virtual void uploadCompleteHandler(bool success,string fileURL) = 0;
    virtual void uploadingProgressHandler(long long uploadedSize,long long expectedSize) = 0;
    
};
#endif /* DMFileUploadManager_hpp */
