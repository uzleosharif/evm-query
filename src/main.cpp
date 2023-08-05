
// SPDX-License-Identifier: MIT

#include "account.hpp"
#include "utils.hpp"

#include "spdlog/spdlog.h"

auto main() -> int {
  eth::Account<eth::RpcClientCpr> account{
      eth::Address{"0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045"}};

  auto exp_balance{account.Balance().and_then(eth::HexToDouble)};
  if (exp_balance) {
    spdlog::info("balance = {}\n", exp_balance.value());
  }
}
