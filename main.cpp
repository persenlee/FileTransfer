#include "cpprest/http_client.h"
#include "cpprest/json.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "DMFile.hpp"
#include "DMFileUploadManager.hpp"
#include "DMFileTransferException.hpp"
using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;


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
    try {
        DMFileUploadManager::defaultManager().uploadFile(file, &test);
    } catch (DMFileTransferException ex) {
        cout << ex.description() ;
    }
    
    return 0;
}
