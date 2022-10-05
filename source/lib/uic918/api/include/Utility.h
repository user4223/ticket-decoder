#pragma once

#include <filesystem>
#include <string>

namespace uic918::api
{
  void dump(std::filesystem::path const &basePath, std::string const &result);
}