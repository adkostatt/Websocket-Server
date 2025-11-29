cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

project ("Websocket-Server")

if (TLS_SUPPORT)
    add_compile_definitions (WEBSOCKET_TLS_SUPPORT)
    include_directories ("../!IncludeFiles")
    link_directories ("../!LibFiles")
endif(TLS_SUPPORT)
unset(TLS_SUPPORT CACHE)

include_directories (".")

file(GLOB WebsocketServerSrc CONFIGURE_DEPENDS
     "Cryptography/*.c"
     "Http/*.cpp"
     "Socket/*.cpp"
     "Websocket/*.cpp"
     "*.cpp"
)
add_executable (Websocket-Server ${WebsocketServerSrc})
