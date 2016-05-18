//
//  DMFileTransferException.hpp
//  FileTransfer
//
//  Created by duomai on 16/5/18.
//
//

#ifndef DMFileTransferException_hpp
#define DMFileTransferException_hpp

#include <iostream>
#include <stdexcept>

class DMFileTransferException : public std::exception
{
public:
    DMFileTransferException(std::string msg) : errMsg(msg){};
    DMFileTransferException(int errCode) : errCode(errCode){};
    DMFileTransferException(std::string msg,int errCode) : errMsg(msg),errCode(errCode){};
    std::string description() { return  "errorCode : " + std::to_string(errCode) + ",errorMsg : " + errMsg + "\n";}
private:
    std::string errMsg;
    int errCode;
};

#endif /* DMFileTransferException_hpp */
