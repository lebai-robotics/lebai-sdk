#pragma once

#include <array>
#include <cctype>
#include <stdexcept>
#include <string>

namespace lebai::base64 {

inline std::string encode(const std::string& input) {
  static constexpr char alphabet[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  std::string output;
  output.reserve(((input.size() + 2) / 3) * 4);

  for (std::size_t i = 0; i < input.size(); i += 3) {
    const auto b0 = static_cast<unsigned char>(input[i]);
    const auto b1 =
        i + 1 < input.size() ? static_cast<unsigned char>(input[i + 1]) : 0;
    const auto b2 =
        i + 2 < input.size() ? static_cast<unsigned char>(input[i + 2]) : 0;

    output.push_back(alphabet[b0 >> 2]);
    output.push_back(alphabet[((b0 & 0x03) << 4) | (b1 >> 4)]);
    output.push_back(i + 1 < input.size()
                         ? alphabet[((b1 & 0x0f) << 2) | (b2 >> 6)]
                         : '=');
    output.push_back(i + 2 < input.size() ? alphabet[b2 & 0x3f] : '=');
  }

  return output;
}

inline std::string decode(const std::string& input) {
  static const auto table = [] {
    std::array<int, 256> values{};
    values.fill(-1);
    const std::string alphabet =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    for (std::size_t i = 0; i < alphabet.size(); ++i) {
      values[static_cast<unsigned char>(alphabet[i])] = static_cast<int>(i);
    }
    return values;
  }();

  std::string compact;
  compact.reserve(input.size());
  for (const auto ch : input) {
    if (!std::isspace(static_cast<unsigned char>(ch))) {
      compact.push_back(ch);
    }
  }

  if (compact.size() % 4 != 0) {
    throw std::runtime_error("invalid base64 length");
  }

  std::string output;
  output.reserve((compact.size() / 4) * 3);

  for (std::size_t i = 0; i < compact.size(); i += 4) {
    const char c0 = compact[i];
    const char c1 = compact[i + 1];
    const char c2 = compact[i + 2];
    const char c3 = compact[i + 3];

    const int v0 = table[static_cast<unsigned char>(c0)];
    const int v1 = table[static_cast<unsigned char>(c1)];
    const int v2 = c2 == '=' ? 0 : table[static_cast<unsigned char>(c2)];
    const int v3 = c3 == '=' ? 0 : table[static_cast<unsigned char>(c3)];
    if (v0 < 0 || v1 < 0 || v2 < 0 || v3 < 0) {
      throw std::runtime_error("invalid base64 character");
    }

    output.push_back(static_cast<char>((v0 << 2) | (v1 >> 4)));
    if (c2 != '=') {
      output.push_back(static_cast<char>(((v1 & 0x0f) << 4) | (v2 >> 2)));
    }
    if (c3 != '=') {
      output.push_back(static_cast<char>(((v2 & 0x03) << 6) | v3));
    }
  }

  return output;
}

}  // namespace lebai::base64
