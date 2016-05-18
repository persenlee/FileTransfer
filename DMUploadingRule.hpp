//
//  DMUploadingRule.hpp
//  FileTransfer
//
//  Created by duomai on 16/5/16.
//
//

#ifndef DMUploadingRule_hpp
#define DMUploadingRule_hpp

#include <iostream>
#include <cpprest/http_client.h>
#include "DMUploadBaseRule.hpp"
#include "DMFile.hpp"
using std::string;
using namespace web;
using namespace web::http;
using namespace web::http::client;
class DMFileUploadHandler;

typedef  enum{
    DMFileUploadFirstTimes,
    DMFileUploadApart,
    DMFileUploadTotal
} DMFileUploadStatus;

struct CheckFileMetaResponseModel
{
    DMFileUploadStatus file_status;
    string file_upload_token;
    int32_t chunk_size;
    int32_t last_ok_chunk_sn;
    string file_url;
    CheckFileMetaResponseModel(){}
    CheckFileMetaResponseModel(DMFileUploadStatus status,string token,int32_t chunkSize,int32_t lastChunkSN,string fileURL)
    : file_status(status) , file_upload_token(token), chunk_size(chunkSize) , last_ok_chunk_sn(lastChunkSN) , file_url(fileURL){}
};

class DMUploadingRule : public DMUploadBaseRule
{
public:
    DMUploadingRule(http_client &client,DMFile const &file,DMFileUploadHandler *handler):_client(client),_file(file),_handler(handler){}
//    void upload();
    virtual bool preProcess() override;
    virtual bool doUpload() override;
    virtual bool endProcess() override;
    void setUserToken(string const &token);
private:
    http_client _client;
    DMFile _file;
    DMFileUploadHandler * _handler;
    CheckFileMetaResponseModel cfmrModel;
    string _userToken;
    bool checkFileMeta();
    bool uploadFileChunk();
};

#endif /* DMUploadingRule_hpp */
