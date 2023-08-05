find_package(spdlog REQUIRED)
find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED)
find_package(cryptopp REQUIRED)

add_executable(eth main.cpp types.cpp utils.cpp)
target_include_directories(eth PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_link_libraries(
  eth
  PRIVATE spdlog::spdlog
          cpr::cpr
          nlohmann_json::nlohmann_json
          Boost::headers
          cryptopp::cryptopp)
