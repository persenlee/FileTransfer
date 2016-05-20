//
//  DMFileDownloadManager.hpp
//  FileTransfer
//
//  Created by duomai on 16/5/12.
//
//

#ifndef DMFileDownloadManager_hpp
#define DMFileDownloadManager_hpp

#include <iostream>
#include <vector>
using namespace std;
class DMFileDownloadHandler;
class DMFileDownloadManager
{
public:
    static DMFileDownloadManager & defaultManager()
    {
        static DMFileDownloadManager manager;
        return manager;
    }
    void  downloadFile (string const & fileURI,DMFileDownloadHandler *handler);
    void  downloadFiles (std::vector<string> const & files,std::vector<DMFileDownloadHandler *> handlers);
private:
};


//下载回调
class DMFileDownloadHandler
{
public:
    virtual void downloadCompleteHandler(bool success,char *fileData) = 0;
    virtual void downloadProgressHandler(int64_t downloadSize, int64_t expectedSize) = 0;
    
};
#endif /* DMFileDownloadManager_hpp */
