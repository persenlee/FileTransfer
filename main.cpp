#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "config.h"
#include "cpprest/http_client.h"
#include "cpprest/json.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "DMFile.hpp"
#include "DMFileUploadManager.hpp"
using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

//#ifdef _WIN32
//int wmain(int argc, wchar_t *args[])
//#else
//int main(int argc, char *args[])
//#endif
//{
//    if(argc != 3)
//    {
//        printf("Usage: BingRequest.exe search_term output_file\n");
//        return -1;
//    }
//    const string_t searchTerm = args[1];
//    const string_t outputFileName = args[2];
//    
//    // Open a stream to the file to write the HTTP response body into.
//    auto fileBuffer = std::make_shared<streambuf<uint8_t>>();
//    file_buffer<uint8_t>::open(outputFileName, std::ios::out).then([=](streambuf<uint8_t> outFile) -> pplx::task<http_response>
//   {
//       *fileBuffer = outFile;
//       
//       // Create an HTTP request.
//       // Encode the URI query since it could contain special characters like spaces.
//       http_client client(U("http://www.bing.com/"));
//       return client.request(methods::GET, uri_builder(U("/search")).append_query(U("q"), searchTerm).to_string());
//   })
//    
//    // Write the response body into the file buffer.
//    .then([=](http_response response) -> pplx::task<size_t>
//          {
//              printf("Response status code %u returned.\n", response.status_code());
//              
//              return response.body().read_to_end(*fileBuffer);
//          })
//    
//    // Close the file buffer.
//    .then([=](size_t)
//          {
//              return fileBuffer->close();
//          })
//    
//    // Wait for the entire response body to be written into the file.
//    .wait();
//    
//    return 0;
//}

class DMUploadingTest : public DMFileUploadHandler
{
public:
    virtual void uploadCompleteHandler(bool success,string fileURL);
    virtual void uploadingProgressHandler(long long uploadedSize,long long expectedSize);
};

void DMUploadingTest::uploadingProgressHandler(long long uploadedSize, long long expectedSize)
{
    cout << "uploadedSize : " << uploadedSize << " expectedSize : " << expectedSize << endl;
}


void DMUploadingTest::uploadCompleteHandler(bool success, string fileURL) {
    cout << "upload file status : " << success << " file url is : " << fileURL << endl;
}


int main(int argc, char *args[])
{
    DMFile file("/Users/duomai/Program/haimi-design/海蜜3.2.0/已确认/iOS/清关保税证明/保税_spec.png");
//    DMFile file("/Users/duomai/Downloads/2015销量前100卖家的调研报告.docx");
//    cout << "file sha1 : " << file.SHA1() << "\ncrc32 : " << file.CRC32() << endl;
    
    
    DMUploadingTest test ;
    DMFileUploadManager::defaultManager().setUserSessionToken("file_serv_test");
    DMFileUploadManager::defaultManager().uploadFile(file, &test);
    return 0;
}
