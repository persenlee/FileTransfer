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

using namespace std;
class DMFileUploadManager {
public:
    DMFileUploadManager();
private:
    string uploadURLStr;
    
};
#endif /* DMFileUploadManager_hpp */
