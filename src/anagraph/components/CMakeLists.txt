add_library(graphcomponents STATIC 
    graph_parser.cpp
    graph_writer.cpp
    unweighted_digraph.cpp
    unweighted_graph.cpp
    unweighted_node.cpp
    weighted_digraph.cpp
    weighted_graph.cpp
    weighted_node.cpp
    unweighted_hetero_node.cpp
    unweighted_hetero_digraph.cpp
    unweighted_hetero_graph.cpp
    weighted_hetero_node.cpp
    weighted_hetero_digraph.cpp
    weighted_hetero_graph.cpp
    weighted_supernode.cpp
    weighted_directed_supergraph.cpp
    weighted_supergraph.cpp
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