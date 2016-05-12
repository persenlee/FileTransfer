//
//  DMFile.hpp
//  FileTransfer
//
//  Created by duomai on 16/5/12.
//
//

#ifndef DMFile_hpp
#define DMFile_hpp
#include <iostream>
#include <fstream>
using namespace std;
typedef enum {
    FILE_SIZE_UNIT_BIT = 1,
    FILE_SIZE_UNIT_BYTE = 8,
    FILE_SIZE_UNIT_KBYTE = 1024,
    FILE_SIZE_UNIT_MBYTE = 1024 * 1024
} DMFileSizeUnit;

class DMFile {
public:
    string fileName;
    string fileURI;
    string SHA1() ;
    int CRC32() ;
    double fileSize(DMFileSizeUnit unit = FILE_SIZE_UNIT_BIT);  //bit
private:
    bool  openFile(ios_base::openmode mode = ifstream::in);
    bool  closeFile();
    ifstream inFileStream;
    long long fileLength();
};
#endif /* DMFile_hpp */
