cmake_minimum_required (VERSION 2.8)

project(FileTransfer)

set (FileTransfer_VERSION_MAJOR 1)
set (FileTransfer_VERSION_MINOR 0)

add_subdirectory(Extra)
set (EXTRA_LIBS ${EXTRA_LIBS} cpprestsdk)
#加入配置头文件
configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)

aux_source_directory(. DIR_SRCS)


add_executable(FileTransfer ${DIR_SRCS})

target_link_libraries(FileTransfer cpprest)
#target_link_libraries(FileTransfer ${EXTRA_LIBS})