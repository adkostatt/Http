find_package(doctest REQUIRED)
file(GLOB_RECURSE TestsSource CONFIGURE_DEPENDS
    *.cpp
    *.c
)

add_executable(HttpTests ${TestsSource})
target_link_libraries(HttpTests PRIVATE Http doctest::doctest)

include(CTest)
include(doctest)
doctest_discover_tests(HttpTests)
