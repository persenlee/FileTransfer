//
//  DMUploadBaseRule.cpp
//  FileTransfer
//
//  Created by duomai on 16/5/16.
//
//

#include "DMUploadBaseRule.hpp"


void DMUploadBaseRule::upload()
{
    preProcess() ? (doUpload() ? endProcess() : NULL) : NULL;
}