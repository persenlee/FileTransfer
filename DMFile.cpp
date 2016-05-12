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

std::streamsize const  bufferSize = PRIVATE_BUFFER_SIZE;
string DMFile::SHA1()
{
    if (!openFile())
    {
        
    }
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    unsigned char buf[bufferSize];
    unsigned char *sha1 = nullptr;
    size_t len = bufferSize;
    while (1)
    {
        inFileStream.read((char *)buf, len);
        if (!inFileStream)
        {
            //read left data
            len = fileLength() % PRIVATE_BUFFER_SIZE;
            inFileStream.read((char *)buf, len);
            SHA1_Update(&ctx, buf, len);
            break;
        }
        SHA1_Update(&ctx, buf, len);
    }
    SHA1_Final(sha1, &ctx);
    string s((char *)sha1);
    closeFile();
    return s;
}


int DMFile::CRC32()
{
    boost::crc_32_type  result;
    if (openFile())
    {
        do
        {
            char  buffer[bufferSize];
            inFileStream.read( buffer, bufferSize );
            result.process_bytes( buffer, inFileStream.gcount() );
        } while (inFileStream);
    }
    closeFile();
    return result.checksum();
}


double DMFile::fileSize(DMFileSizeUnit unit)
{
    return (double)fileLength() / unit ;
}


long long DMFile::fileLength()
{
    int length = 0;
    if (openFile(ifstream::binary)) {
        inFileStream.seekg(inFileStream.end);
        length = inFileStream.tellg();
        inFileStream.seekg(inFileStream.beg);
    }
    closeFile();
    return length;
}


bool  DMFile::openFile(ios_base::openmode mode)
{
    if (!inFileStream.is_open()) {
        inFileStream.open(fileURI,mode);
        return inFileStream.is_open();
    } else {
        return true;
    }
    
}

bool  DMFile::closeFile()
{
    if (inFileStream.is_open()) {
        inFileStream.close();
        return !inFileStream.is_open();
    } else {
        return  true;
    }
    
}