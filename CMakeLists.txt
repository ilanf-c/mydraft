cmake_minimum_required(VERSION 3.16)
project(draft CXX)

set(CMAKE_CXX_COMPILER "clang++")

# designate the path of Qt (if cmake is not able to find it)
# set(CMAKE_PREFIX_PATH "/opt/Qt/6.7.3/gcc_64/lib/cmake")

# look for Qt modules
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# set auto processing of Qt MOC, UIC and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
endif()

include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(draft
    src/main.cpp
    src/MainWindow.cpp
    include/MainWindow.hpp
    include/FileExplorer.hpp
)

# link Qt libraries
target_link_libraries(draft
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

target_compile_options(draft
    PRIVATE
        -g # generate debug info
        -O0 # no optimization
        -Wall # set on all warnings
        -fno-limit-debug-info # disable debug info limit
)