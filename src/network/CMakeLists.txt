file(GLOB NETWORK_SRC *.cpp)
add_library(network ${NETWORK_SRC})
target_include_directories(network PUBLIC ${PROJECT_SOURCE_DIR}/include)
