file(GLOB_RECURSE LibSource CONFIGURE_DEPENDS
    *.cpp
    *.c
)

add_library(Http ${LibSource})
target_include_directories(Http PUBLIC ${CMAKE_SOURCE_DIR}/include)
