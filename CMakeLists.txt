cmake_minimum_required(VERSION 3.10)
project(gstreamer_framework)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

find_package(yaml-cpp REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

include_directories(
    ${GST_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
)

link_directories(${GST_LIBRARY_DIRS})

file(GLOB SRC
    src/*.cpp
    src/*.cc
)

add_executable(app ${SRC})

target_link_libraries(app
    ${GST_LIBRARIES}
    yaml-cpp
    gRPC::grpc++
    protobuf::libprotobuf
)