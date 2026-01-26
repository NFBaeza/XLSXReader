cmake_minimum_required(VERSION 3.16)

project(XLSXREADER VERSION 1.0 LANGUAGES CXX)


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


add_subdirectory(${qxlsx_SOURCE_DIR}/QXlsx ${qxlsx_BINARY_DIR}/QXlsx)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


set(SOURCES
    src/banks/biceBank.cpp
    src/banks/santanderBank.cpp
    src/bank.cpp
    src/bankFactory.cpp
)

# Header files
set(HEADERS
    include/banks/biceBank.h
    include/banks/santanderBank.h
    include/bank.h
    include/bankFactory.h
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
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Sql
    Qt6::Core
    Qt6::Gui
    QXlsx
)