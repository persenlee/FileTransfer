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
#include <vector>
using namespace std;

typedef enum {
    FILE_SIZE_UNIT_BIT = 1,
    FILE_SIZE_UNIT_BYTE = 8,
    FILE_SIZE_UNIT_KBYTE = 1024,
    FILE_SIZE_UNIT_MBYTE = 1024 * 1024
} DMFileSizeUnit;

class DMFile {
public:
    explicit DMFile(string fileURI) : fileURI(fileURI)
    {
        std::size_t found = fileURI.find_last_of("/\\");
        fileName = fileURI.substr(found+1);
    };
    DMFile(DMFile const &);
    DMFile(){}
    DMFile & operator=(DMFile const &);
    string fileName;
    string fileURI;
    string SHA1();
    uint32_t CRC32();
    double fileSize(DMFileSizeUnit unit = FILE_SIZE_UNIT_BYTE);  //bit
    bool readFile(char* buff, streamsize buffSize);
    bool readFile2Vector(std::vector<unsigned char> &vector,streamsize size);
    bool  openFile(ios_base::openmode mode = ifstream::in);
    bool  closeFile();
    bool isOpen();
    void moveFileReadPos(int64_t pos);
    int64_t fileLength();
private:
    
    ifstream inFileStream;
    
};
#endif /* DMFile_hpp */
