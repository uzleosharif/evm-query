
// SPDX-License-Identifier: MIT

#include "types.hpp"

#include <algorithm>
#include <string>

#include "utils.hpp"
//
#include "spdlog/spdlog.h"

namespace eth {

Address::Address() noexcept {
  std::ranges::fill(value_, '0');
}

Address::Address(std::string_view addr_str) noexcept {
  if (auto validity{Valid(addr_str)}; not validity) {
    // TODO: red color for emphasis on ERROR
    spdlog::error("{}. Cause: {}. addr_str={}\n",
                  "Address(std::string_view) failed", validity.error(),
                  addr_str);
    std::ranges::fill(value_, 'f');
  }

  std::copy(std::begin(addr_str) + 2, std::end(addr_str), std::begin(value_));
}

auto Address::Str() const noexcept -> std::string {
  return (std::string{"0x"} +
          std::string{std::begin(value_), std::end(value_)});
}

}  // namespace eth
