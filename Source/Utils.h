#pragma once

namespace Utils {
    inline std::string formatIpBytes(uint32_t ipAddressBytes) {
        std::ostringstream ipstream;

        ipstream << ((ipAddressBytes >> 24) & 255) << "."
                 << ((ipAddressBytes >> 16) & 255) << "."
                 << ((ipAddressBytes >> 8) & 255) << "."
                 << (ipAddressBytes & 255);

        return ipstream.str();
    }
} // namespace Utils
