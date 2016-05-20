//
//  DMUploadingRule.cpp
//  FileTransfer
//
//  Created by duomai on 16/5/16.
//
//

#include "DMUploadingRule.hpp"
#include "DMFileUploadManager.hpp"
#include "DMFileTransferException.hpp"
#include "DMFileTransferDef.h"
using namespace web;
using namespace web::json;

void logRequest(http_request request);
void logResponse(http_response response);

void DMUploadingRule::setUserToken(string const &token)
{
    _userToken = token;
}

#pragma mark - Ovrride Methods
bool DMUploadingRule::preProcess()
{
    return checkFileMeta();
}

bool DMUploadingRule::doUpload()
{
    return uploadFileChunk();
}

bool DMUploadingRule::endProcess()
{
    return true;
}

#pragma mark - Private Methods

bool DMUploadingRule::checkFileMeta() 
{
    json::value params = json::value::object();
    utility::string_t file_hash(U("file_hash"));
    params[file_hash] = json::value(_file.SHA1());
    
    utility::string_t file_crc(U("file_crc"));
    params[file_crc] = json::value(_file.CRC32());
    
    utility::string_t file_name(U("file_name"));
    params[file_name] = json::value(_file.fileName);
    
    utility::string_t total_size(U("total_size"));
    params[total_size] = json::value(_file.fileSize());
    

    const method mtd = methods::POST;
    const utility::string_t field_name1 = U("user_session_token");
    const utility::string_t value1 = _userToken;
    
    http_request request(mtd);
    request.set_request_uri(U("check_file_meta"));
    request.headers().add(field_name1, value1);
    request.set_body(params);
    request.headers().add(U("User-Agent"), "DMFileUploadManager");
    
    pplx::task<http_response> responsetTask = _client.request(request);

    logRequest(request);

    http_response resp ;
    try {
        resp = responsetTask.get();
    } catch (http_exception ex) {
        DMFileTransferException dmException((int)DMFileTransfer_CouldNotGetResponse);
        throw dmException;
    }
   
    logResponse(resp);
    
    if (resp.status_code() == 200)
    {
        pplx::task<json::value> jsonTask =  resp.extract_json();
        json::value  result= jsonTask.get();
        
        json::value file_status =  result[U("file_status")];    //
        json::value file_upload_token =  result[U("file_upload_token")];
        json::value file_url =  result[U("file_url")];
        json::value last_ok_chunk_sn =  result[U("last_ok_chunk_sn")];
        json::value chunk_size =  result[U("chunk_size")];
        
        cfmrModel = CheckFileMetaResponseModel((DMFileUploadStatus)file_status.as_integer(),
                                               !file_upload_token.is_null() ? file_upload_token.as_string() : "",
                                               !chunk_size.is_null() ? chunk_size.as_integer() : 0,
                                               !last_ok_chunk_sn.is_null() ? last_ok_chunk_sn.as_integer() : 0,
                                               !file_url.is_null() ? file_url.as_string() : "");
        switch (cfmrModel.file_status)
        {
            case 0:
            {
                _handler->uploadingProgressHandler(0, _file.fileSize());
            }
                break;
            case 1:
            {
                _handler->uploadingProgressHandler(cfmrModel.chunk_size * cfmrModel.last_ok_chunk_sn, _file.fileSize());
            }
                break;
            case 2:
            {
                
                _handler->uploadCompleteHandler(true,file_url.as_string());
                _handler->uploadingProgressHandler(_file.fileSize(), _file.fileSize());
                return false;
            }
                break;
            default:
                break;
        }
        return true;

    }
    else
    {
        _handler->uploadCompleteHandler(false, "");
        return false;
    }
}

bool DMUploadingRule::uploadFileChunk()
{
    bool success = false;
    const method mtd = methods::POST;
    const utility::string_t field_name1 = U("upload_token");
    const utility::string_t value1 = cfmrModel.file_upload_token;
    
    http_request request(mtd);
    request.set_request_uri(U("upload_file_chunk"));
    request.headers().add(field_name1, value1);
    request.headers().set_content_length(INTMAX_MAX);
    request.headers().add(U("User-Agent"), "DMFileUploadManager");

    int64_t File_Size = _file.fileLength();
    _file.openFile(/*iostream::binary*/);
    //续传
    int64_t uploadedSize = cfmrModel.last_ok_chunk_sn * cfmrModel.chunk_size;
    if (uploadedSize > 0) {
        _file.moveFileReadPos(uploadedSize);
    }
    do {
        int  request_again_times = 0;
        
        int  readSize = cfmrModel.chunk_size;
        if (uploadedSize + cfmrModel.chunk_size > File_Size) {
            readSize = File_Size %  cfmrModel.chunk_size;
        }
        
        std::vector<unsigned char> body;
        _file.readFile2Vector(body, readSize);
        request.set_body(body);
    request_again:
        request_again_times ++;
        
        logRequest(request);

        pplx::task<http_response> responsetTask = _client.request(request);
        http_response resp ;
        try {
            resp = responsetTask.get();
        } catch (http_exception ex) {
            DMFileTransferException dmException((int)DMFileTransfer_CouldNotGetResponse);
            throw dmException;
        }
        
        logResponse(resp);
        if (resp.status_code() == 200)
        {
            pplx::task<json::value> jsonTask =  resp.extract_json();
            json::value  result= jsonTask.get();
            
            cout << "response body : " << result.serialize() << endl;
            json::value file_status =  result[U("file_status")];    //
            json::value chunk_status =  result[U("chunk_status")];
            json::value file_url =  result[U("file_url")];
    
            switch (file_status.as_integer())
            {
                case 1: //正在上传
                {
                    if (chunk_status.as_integer() == -1) { // 标识数据块保存失败
                        goto request_again;
                    } else if(chunk_status.as_integer() == 0){
                         uploadedSize += readSize;
                        _handler->uploadingProgressHandler(uploadedSize, File_Size);
                    }
                }
                    break;
                case 2: //上传成功
                {
                   uploadedSize += readSize;
                    _handler->uploadingProgressHandler(uploadedSize, File_Size);
                    _handler->uploadCompleteHandler(true,file_url.as_string());
                    success = true;
                }
                    break;
                case -1: //文件上传失败
                {
                    _handler->uploadCompleteHandler(false, "");;
                }
                    break;
                default:
                    break;
            }
            
        }
        else
        {
            _handler->uploadCompleteHandler(false, "");
        }
    } while (uploadedSize < File_Size);
    _file.closeFile();
    return success;
}

void logRequest(http_request request)
{
    cout << "---------------------request start------------------" << endl;
    cout << request.to_string() << endl;
    cout << "---------------------request   end------------------" << endl << endl;
}

void logResponse(http_response response)
{
    cout << "++++++++++++++++++++response start++++++++++++++++++++" << endl;
    cout << response.to_string() << endl;
    cout << "++++++++++++++++++++response   end++++++++++++++++++++" << endl << endl;
}