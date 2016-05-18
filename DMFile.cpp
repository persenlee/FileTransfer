//
//  DMFile.cpp
//  FileTransfer
//
//  Created by duomai on 16/5/12.
//
//

#include "DMFile.hpp"
#include <openssl/sha.h>
#include <boost/crc.hpp>
#ifndef PRIVATE_BUFFER_SIZE
#define PRIVATE_BUFFER_SIZE  1024
#endif


using namespace std;
DMFile::DMFile(DMFile const &sFile)
{
    this->fileName = sFile.fileName;
    this->fileURI = sFile.fileURI;
}


 DMFile & DMFile::operator=(DMFile const &sFile)
{
    this->fileName = sFile.fileName;
    this->fileURI = sFile.fileURI;
    return *this;
}

string bytestohexstring(char* bytes,int bytelength)
{
    string str("");
    string str2("0123456789abcdef");
    for (int i=0;i<bytelength;i++) {
        int b;
        b = 0x0f&(bytes[i]>>4);
        str.append(1,str2.at(b));
        b = 0x0f & bytes[i];
        str.append(1,str2.at(b));
    }  
    return str;  
}

std::streamsize const  bufferSize = PRIVATE_BUFFER_SIZE;
string DMFile::SHA1()
{
    if (!openFile())
    {
        
    }
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    unsigned char buf[bufferSize];
    unsigned char sha1[160];
    size_t len = bufferSize;
    while (1)
    {
        inFileStream.read((char *)buf, len);
        if (!inFileStream)
        {
            //read left data
            len = fileLength() % PRIVATE_BUFFER_SIZE;
//            inFileStream.read((char *)buf, len);
            SHA1_Update(&ctx, buf, len);
            break;
        }
        SHA1_Update(&ctx, buf, len);
    }
    SHA1_Final(sha1, &ctx);
    string hexStr = bytestohexstring((char *)sha1, 20);
    closeFile();
    return hexStr;
}


uint32_t DMFile::CRC32()
{
    char buffer[bufferSize];
    size_t len = bufferSize;
    boost::crc_32_type  result;

    if (openFile()) {
        while (1)
        {
            inFileStream.read((char *)buffer, len);
            if (!inFileStream)
            {
                len = fileLength() % PRIVATE_BUFFER_SIZE;
                result.process_bytes(buffer, len);
                break;
            }
            result.process_bytes(buffer, len);
        }

    }
    closeFile();
    return result.checksum();
}


double DMFile::fileSize(DMFileSizeUnit unit)
{
    return (double)fileLength() * 8 / unit ;
}


int64_t DMFile::fileLength()
{
    int64_t length = 0;
    ifstream myfile (fileURI, ios::binary);
    streampos begin,end;
    begin = myfile.tellg();
    myfile.seekg (0, ios::end);
    end = myfile.tellg();
    length = end - begin;
    myfile.close();
    return length;
}


bool  DMFile::openFile(ios_base::openmode mode)
{
    if (!isOpen()) {
        inFileStream.open(fileURI,mode);
        return isOpen();
    } else {
        return true;
    }
    
}

bool  DMFile::closeFile()
{
    if (isOpen()) {
        inFileStream.close();
        return !isOpen();
    } else {
        return  true;
    }
    
}


bool DMFile::readFile(char* buff, streamsize buffSize)
{
    inFileStream.read(buff, bufferSize);
    return  !!inFileStream;
}

bool DMFile::isOpen()
{
    return inFileStream.is_open();
}


void DMFile::moveFileReadPos(int64_t pos)
{
    inFileStream.seekg(pos);
}


bool DMFile::readFile2Vector(std::vector<unsigned char> &vector,streamsize size)
{
    for (int i = 0; i < size; i++)
    {
        char ch;
        inFileStream >> std::noskipws >> ch;
        vector.push_back(ch);
    }
    return  !!inFileStream;
}
