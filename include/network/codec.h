#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace network {

struct PacketHeader {
    uint32_t len;
    uint16_t ver;
    uint16_t cmd;
};

class Codec {
public:
    static constexpr size_t HEADER_SIZE = 8;
    static constexpr size_t MAX_PAYLOAD = 65536;

    static bool Decode(std::vector<char>& buffer, PacketHeader& header, std::string& payload);
    static std::string Encode(uint16_t cmd, const std::string& payload);
};

}
