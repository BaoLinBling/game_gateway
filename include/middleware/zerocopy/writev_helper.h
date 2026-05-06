#pragma once
#include <sys/uio.h>
#include <string>

inline ssize_t writev_send(int fd, const std::string& head, const std::string& body) {
    struct iovec iov[2];

    iov[0].iov_base = (void*)head.data();
    iov[0].iov_len = head.size();

    iov[1].iov_base = (void*)body.data();
    iov[1].iov_len = body.size();

    return writev(fd, iov, 2);
}
