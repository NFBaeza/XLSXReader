cmake_minimum_required(VERSION 3.16)

project(XLSXREADER VERSION 1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON) 


find_package(Qt6 REQUIRED COMPONENTS Core Gui Sql)

include(FetchContent)

FetchContent_Declare(
    QXlsx
    GIT_REPOSITORY https://github.com/QtExcel/QXlsx.git
    GIT_TAG master
)
FetchContent_MakeAvailable(QXlsx)

FetchContent_Declare(
    libxls
    GIT_REPOSITORY https://github.com/libxls/libxls.git
    GIT_TAG v1.6.2
)
FetchContent_MakeAvailable(libxls)

# libxls has no CMakeLists.txt (autotools), so we build it manually.
# FetchContent_MakeAvailable still sets libxls_SOURCE_DIR.
if(NOT EXISTS "${libxls_SOURCE_DIR}/config.h")
    file(WRITE "${libxls_SOURCE_DIR}/config.h"
        "#define HAVE_ICONV 1\n"
        "#define ICONV_CONST\n"
        "#define PACKAGE_VERSION \"1.6.2\"\n"
        "#define PACKAGE_NAME \"libxls\"\n"
    )
endif()

add_library(xls STATIC
    ${libxls_SOURCE_DIR}/src/xls.c
    ${libxls_SOURCE_DIR}/src/ole.c
    ${libxls_SOURCE_DIR}/src/xlstool.c
    ${libxls_SOURCE_DIR}/src/locale.c
    ${libxls_SOURCE_DIR}/src/endian.c
)
set_target_properties(xls PROPERTIES AUTOMOC OFF)
target_include_directories(xls PUBLIC
    ${libxls_SOURCE_DIR}/include
    ${libxls_SOURCE_DIR}
)


add_subdirectory(${qxlsx_SOURCE_DIR}/QXlsx ${qxlsx_BINARY_DIR}/QXlsx)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


set(SOURCES
    src/banks/biceBank.cpp
    src/banks/santanderBank.cpp
    src/banks/wiseBank.cpp
    src/bank.cpp
    src/bankFactory.cpp
    src/simpleClassifier.cpp
    src/banks/estadoBank.cpp
    src/banks/chileBank.cpp
)

# Header files
set(HEADERS
    include/simpleClassifier.h
    include/banks/biceBank.h
    include/banks/santanderBank.h
    include/banks/wiseBank.h
    include/banks/estadoBank.h
    include/bank.h
    include/bankFactory.h 
    include/banks/chileBank.h
)
    

add_library(${PROJECT_NAME} STATIC
    ${HEADERS}
    ${SOURCES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${qxlsx_SOURCE_DIR}/QXlsx/header
    ${libxls_SOURCE_DIR}/include
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Sql
    Qt6::Core
    Qt6::Gui
    QXlsx
    xls
)

# Test executable
add_executable(test_xlsxreader main.cpp)

target_link_libraries(test_xlsxreader PRIVATE
    ${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Sql
    QXlsx
    xls
)