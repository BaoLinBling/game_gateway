#muduo 网络模块编译运行的核心文件，必须保留
find_path(MUDUO_INCLUDE_DIR muduo/net/TcpServer.h
    /usr/local/include
)

find_library(MUDUO_NET_LIBRARY muduo_net
    /usr/local/lib
)

find_library(MUDUO_BASE_LIBRARY muduo_base
    /usr/local/lib
)

set(MUDUO_LIBRARIES
    ${MUDUO_NET_LIBRARY}
    ${MUDUO_BASE_LIBRARY}
)

include_directories(${MUDUO_INCLUDE_DIR})
