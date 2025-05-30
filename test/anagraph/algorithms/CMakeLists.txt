include(GoogleTest)

function(add_algorithm_test_executable name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} GTest::gtest_main graphalgorithms)
    target_compile_features(${name} PUBLIC cxx_std_20)
    target_compile_definitions(${name} PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
    if (USE_ASAN)
        target_compile_options(${name} PUBLIC -Wall -fsanitize=address)
        target_link_options(${name} PUBLIC -fsanitize=address)
    else()
        target_compile_options(${name} PUBLIC -Wall)
    endif()
    gtest_discover_tests(${name})
endfunction(add_algorithm_test_executable name)

add_algorithm_test_executable(pagerank_test)
add_algorithm_test_executable(similarity_test)