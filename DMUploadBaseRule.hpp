//
//  DMUploadBaseRule.hpp
//  FileTransfer
//
//  Created by duomai on 16/5/16.
//
//

#ifndef DMUploadBaseRule_hpp
#define DMUploadBaseRule_hpp
#include <iostream>

class DMUploadBaseRule
{
public :
    void upload();
protected:
   virtual bool preProcess() = 0;
   virtual bool doUpload() = 0;
   virtual bool endProcess() = 0;
};
#endif /* DMUploadBaseRule_hpp */
