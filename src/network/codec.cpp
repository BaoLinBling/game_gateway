#include "network/codec.h"
#include <arpa/inet.h>
#include <cstring>

namespace network {

bool Codec::Decode(std::vector<char>& buffer, PacketHeader& header, std::string& payload) {
    if (buffer.size() < HEADER_SIZE) return false;

    uint32_t len = ntohl(*(uint32_t*)&buffer[0]);
    if (len > HEADER_SIZE + MAX_PAYLOAD) {
        buffer.clear();
        return false;
    }

    if (buffer.size() < len) return false;

    header.len = len;
    header.ver = ntohs(*(uint16_t*)&buffer[4]);
    header.cmd = ntohs(*(uint16_t*)&buffer[6]);

    payload.assign(buffer.begin() + HEADER_SIZE, buffer.begin() + len);
    buffer.erase(buffer.begin(), buffer.begin() + len);
    return true;
}

std::string Codec::Encode(uint16_t cmd, const std::string& payload) {
    uint32_t len = HEADER_SIZE + payload.size();
    std::string out;
    out.resize(len);

    *(uint32_t*)&out[0] = htonl(len);
    *(uint16_t*)&out[4] = htons(1);
    *(uint16_t*)&out[6] = htons(cmd);
    memcpy(&out[8], payload.data(), payload.size());
    return out;
}

}
