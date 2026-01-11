#pragma once

#include <format>
#include <random>
#include <ranges>
#include <vector>

template <std::ranges::range R>
  requires(not std::same_as<R, std::string>)
struct std::formatter<R> : std::formatter<std::string> {
  auto format(const R &range, auto &ctx) const {
    std::string s = "[";

    bool first = true;
    for (auto &&elem : range) {
      if (!first)
        s += ", ";
      first = false;
      s += std::format("{}", elem);
    }

    s += "]";
    return std::formatter<std::string>::format(s, ctx);
  }
};