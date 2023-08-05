
// SPDX-License-Identifier: MIT

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <type_traits>

#include "tl/expected.hpp"
#include "utils.hpp"

namespace eth {

template <RpcClient client_t>
class Account final {
 public:
  Account(Address&& address) noexcept : address_{address} {}
  Account(const Account&) = delete;
  auto operator=(const Account&) -> Account& = delete;
  Account(Account&&) noexcept = default;
  auto operator=(Account&&) noexcept -> Account& = default;
  ~Account() = default;

  [[nodiscard]] auto Balance() const noexcept
      -> tl::expected<std::string, std::string> {
    auto balance_response{
        rpc_client_.SendRequest("eth_getBalance", {address_.Str(), "latest"})};
    if (not balance_response) {
      return tl::unexpected{"JSON-RPC error: " + balance_response.error()};
    }

    auto& balance{balance_response.value()};
    if (balance.contains("error")) {
      return tl::unexpected{"JSON-RPC error: " +
                            std::string{balance["error"]["message"]}};
    }

    return balance["result"];
  }

 private:
  Address address_{};
  client_t rpc_client_{};
};

static_assert(not std::is_copy_assignable_v<Account<RpcClientCpr>>);
static_assert(not std::is_copy_constructible_v<Account<RpcClientCpr>>);
static_assert(std::is_move_assignable_v<Account<RpcClientCpr>>);
static_assert(std::is_move_constructible_v<Account<RpcClientCpr>>);
}  // namespace eth

#endif
