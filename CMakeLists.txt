cmake_minimum_required(VERSION 3.16)
project(QRCodeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable Qt6 MOC (Meta-Object Compiler)
set(CMAKE_AUTOMOC ON)

# Find libqrencode
find_package(PkgConfig REQUIRED)
pkg_check_modules(QRENCODE REQUIRED libqrencode)

add_executable(QRCodeGenerator main.cpp)

target_link_libraries(QRCodeGenerator 
    Qt6::Core 
    Qt6::Widgets
    ${QRENCODE_LIBRARIES}
)

target_include_directories(QRCodeGenerator PRIVATE ${QRENCODE_INCLUDE_DIRS})
target_compile_options(QRCodeGenerator PRIVATE ${QRENCODE_CFLAGS_OTHER})