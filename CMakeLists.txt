cmake_minimum_required(VERSION 3.16)
project(bencoder LANGUAGES C VERSION 1.0)

include(GNUInstallDirs)

add_library(bencoder SHARED
    src/bencoder.c
)
add_library(bencoderstatic STATIC
    src/bencoder.c
)
set_target_properties(bencoderstatic PROPERTIES OUTPUT_NAME bencoder)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS bencoder bencoderstatic)
install(FILES include/bencoder.h DESTINATION include)

if(UNIX AND NOT DO_NOT_LDCONFIG)
    install(CODE "execute_process(COMMAND ldconfig)")
endif()
