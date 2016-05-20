cmake_minimum_required (VERSION 2.8)

project(FileTransfer)


if(IOS)
	set(IOS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Build_iOS")
	set(Boost_INCLUDE_DIR "${IOS_SOURCE_DIR}/boost.framework/Headers")
    set(Boost_FRAMEWORK "-F ${IOS_SOURCE_DIR} -framework boost")
    set(OPENSSL_FOUND 1)
    set(OPENSSL_INCLUDE_DIR "${IOS_SOURCE_DIR}/openssl/include")
    set(OPENSSL_LIBRARIES
    	"${IOS_SOURCE_DIR}/openssl/lib/libcrypto.a"
    	"${IOS_SOURCE_DIR}/openssl/lib/libssl.a"
    )
    set(CPPREST_LIBRARIES  "${IOS_SOURCE_DIR}/libcpprest.a")
    # The cxx_flags must be reset here, because the ios-cmake toolchain file unfortunately sets "-headerpad_max_install_names" which is not a valid clang flag.
  set(CMAKE_CXX_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
elseif(ANDROID)
	set(Boost_COMPILER "-clang")
  set(Boost_USE_STATIC_LIBS ON)
  if(ARM)
    set(BOOST_ROOT "${CMAKE_BINARY_DIR}/../Boost-for-Android/build")
    set(BOOST_LIBRARYDIR "${CMAKE_BINARY_DIR}/../Boost-for-Android/build/lib")
  else()
    set(BOOST_ROOT "${CMAKE_BINARY_DIR}/../Boost-for-Android-x86/build")
    set(BOOST_LIBRARYDIR "${CMAKE_BINARY_DIR}/../Boost-for-Android-x86/build/lib")
  endif()
  find_host_package(Boost 1.55 EXACT REQUIRED COMPONENTS random system thread filesystem chrono atomic)

  set(OPENSSL_FOUND 1)
  if(ARM)
    set(OPENSSL_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../openssl/armeabi-v7a/include")
    set(OPENSSL_LIBRARIES
      "${CMAKE_BINARY_DIR}/../openssl/armeabi-v7a/lib/libssl.a"
      "${CMAKE_BINARY_DIR}/../openssl/armeabi-v7a/lib/libcrypto.a"
      )
  else()
    set(OPENSSL_INCLUDE_DIR "${CMAKE_BINARY_DIR}/../openssl/x86/include")
    set(OPENSSL_LIBRARIES
      "${CMAKE_BINARY_DIR}/../openssl/x86/lib/libssl.a"
      "${CMAKE_BINARY_DIR}/../openssl/x86/lib/libcrypto.a"
      )
  endif()

  if(ARM)
    set(LIBCXX_STL "${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.8/libs/armeabi-v7a/thumb/libgnustl_static.a")
  else()
    set(LIBCXX_STL "${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.8/libs/x86/libgnustl_static.a")
  endif()
  # These are used in the shared library case
  set(ANDROID_STL_FLAGS
    ${LIBCXX_STL}
    atomic
    dl
    gcc
    c
    m
    -nodefaultlibs
    )

  option(BUILD_SHARED_LIBS "Build shared Libraries." OFF)
  set(BUILD_SAMPLES OFF)
  option(BUILD_TESTS "Build tests." ON)
  
elseif(UNIX) # This includes OSX

endif()

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
