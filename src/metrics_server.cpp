#include <thread>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "middleware/monitor/prometheus.h"

void StartMetricsServer() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return;

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sockfd);
        return;
    }

    listen(sockfd, 10);

    while (true) {
        int client = accept(sockfd, nullptr, nullptr);
        if (client < 0) continue;

        std::string body = middleware::monitor::Metrics::Instance().Serialize();
        std::string resp =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n\r\n" + body;

        send(client, resp.data(), resp.size(), 0);
        close(client);
    }
}
