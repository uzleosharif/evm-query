

#include "utils.hpp"

#include <regex>

#include "boost/multiprecision/cpp_bin_float.hpp"
#include "boost/multiprecision/cpp_int.hpp"
#include "fmt/core.h"

namespace eth {

auto HexToDouble(std::string_view hex_str) noexcept
    -> tl::expected<double, std::string> {
  static const boost::multiprecision::uint256_t kDecimalFactor{
      std::pow(10, 18)};

  try {
    auto result =
        static_cast<boost::multiprecision::cpp_bin_float_100>(
            boost::multiprecision::uint256_t{hex_str}) /
        static_cast<boost::multiprecision::cpp_bin_float_100>(kDecimalFactor);
    return result.convert_to<double>();
  } catch (const std::exception& ex) {
    return tl::unexpected{
        fmt::format("division failed with exception: {}", ex.what())};
  }
}

auto Valid(std::string_view addr_str) noexcept
    -> tl::expected<void, std::string> {
  static constexpr std::string_view pattern_str{"^0x[0-9a-fA-F]{40}$"};
  static const std::regex pattern{std::string{pattern_str}};

  if (not addr_str.starts_with("0x")) {
    return tl::unexpected{"address string not formatted properly"};
  }
  if (not(addr_str.size() == (Address::kNumBytes + 2))) {
    return tl::unexpected{"address string has incorrect size"};
  }
  if (not std::regex_match(std::string{addr_str}, pattern)) {
    return tl::unexpected{"address string is not alphanumeric"};
  }

  return {};
}

auto RpcClientCpr::SendRequest(std::string_view method,
                               nlohmann::json&& params) const noexcept
    -> tl::expected<nlohmann::json, std::string> {
  nlohmann::json request{
      {"jsonrpc", "2.0"}, {"method", method}, {"params", params}, {"id", 1}};

  cpr::Response response{};
  try {
    response = cpr::Post(cpr::Url{kEndpointUrl}, cpr::Body{request.dump()},
                         kCprHeader);
  } catch (const std::exception& ex) {
    return tl::unexpected{std::string{"caught exception while sending Post "
                                      "request via cpr. Exception: "} +
                          std::string{ex.what()}};
  }

  nlohmann::json result{};
  try {
    result = nlohmann::json::parse(response.text);
  } catch (std::exception& ex) {
    return tl::unexpected{
        std::string{"caught exception while json::parse with msg: "} +
        std::string{ex.what()}};
  }

  return result;
}

}  // namespace eth
