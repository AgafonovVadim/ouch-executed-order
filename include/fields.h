#pragma once

#include <cmath>
#include <string>
#include <vector>

inline unsigned integer_parser(const std::vector<unsigned char> & message, size_t & current_index)
{
    unsigned result = (message[current_index] << 24) + (message[current_index + 1] << 16) + (message[current_index + 2] << 8) + message[current_index + 3];
    current_index += 4;
    return result;
}

inline std::string string_parser(const std::vector<unsigned char> & message, size_t & current_index, const size_t len)
{
    std::string result = std::string{reinterpret_cast<const char *>(&message[current_index]), len};
    current_index += len;
    return result.substr(0, result.find_last_not_of(' ') + 1);
}

inline void decode_mmt(const std::vector<unsigned char> & message, char (&mmt)[15], size_t & current_index)
{
#define FIELD(_, default_value, index) mmt[index] = default_value;
#include "mmt_default_fields.inl"
#define FIELD(_, index) mmt[index] = message[current_index++];
#include "mmt_refactored_fields.inl"
}
