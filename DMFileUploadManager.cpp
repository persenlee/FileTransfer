//
//  DMFileUploadManager.cpp
//  FileTransfer
//
//  Created by duomai on 16/5/12.
//
//

#include "DMFileUploadManager.hpp"


void DMFileUploadManager::setUploadURLStr(string urlStr)
{
    _uploadURLStr = urlStr;
    _httpClient = http_client(_uploadURLStr);
}

void DMFileUploadManager::setUserSessionToken(string token)
{
    _user_session_token = token;
    
}

void DMFileUploadManager::uploadFile(DMFile const &file,DMFileUploadHandler *handler)
{
    DMUploadingRule rule(_httpClient,file,handler);
    rule.setUserToken(_user_session_token);
    rule.upload();
}

void DMFileUploadManager::uploadFileWithFileURI(string const & fileURI,DMFileUploadHandler *handler)
{
    DMFile file(fileURI);
    uploadFile(file, handler);
}