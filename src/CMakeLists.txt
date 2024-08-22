add_library(graphcomponents STATIC 
    weighted_graph.cpp
    unweighted_graph.cpp
    graph_parser.cpp
    graph_writer.cpp
    heterogeneous_graph.cpp
    supergraph.cpp
)
target_link_libraries(graphcomponents PUBLIC spdlog::spdlog)
target_include_directories(graphcomponents PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(graphcomponents PUBLIC cxx_std_20)
if (USE_ASAN)
    target_compile_options(graphcomponents PUBLIC -Wall -fsanitize=address)
    target_link_options(graphcomponents PUBLIC -fsanitize=address)
else()
    target_compile_options(graphcomponents PUBLIC -Wall)
endif()

add_library(graphalgorithms STATIC
    pagerank.cpp
)
target_link_libraries(graphalgorithms PUBLIC spdlog::spdlog)
target_include_directories(graphalgorithms PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(graphalgorithms PUBLIC cxx_std_20)
if (USE_ASAN)
    target_compile_options(graphalgorithms PUBLIC -Wall -fsanitize=address)
    target_link_options(graphalgorithms PUBLIC -fsanitize=address)
else()
    target_compile_options(graphalgorithms PUBLIC -Wall)
endif()