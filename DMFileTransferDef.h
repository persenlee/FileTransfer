//
//  DMFileTransferDef.h
//  FileTransfer
//
//  Created by duomai on 16/5/16.
//
//

#ifndef DMFileTransferDef_h
#define DMFileTransferDef_h

typedef enum{
    DMFileUpload_ConnectSeverFaild,   //无法连接服务器
    DMFileUpload_FileNotExisit,
    DMFileTransfer_CouldNotGetResponse,
    DMFileTransfer_TimeOut
} DMFileTransfedrErrorCode;

#endif /* DMFileTransferDef_h */
