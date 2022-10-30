
#include "../include/Logging.h"

#include <filesystem>

INITIALIZE_EASYLOGGINGPP;

namespace utility
{
  Logger LoggerFactory::fromPath(std::string fullPath)
  {
    auto name = std::filesystem::path(fullPath).stem().string();
    el::Loggers::getLogger(name);
    return Logger{std::move(name)};
  }

  LoggerFactory LoggerFactory::create()
  {
    el::Configurations config;
    config.setToDefault();
    config.setGlobally(el::ConfigurationType::Format, "[%levshort %datetime{%Y-%M-%dT%H:%m:%s} %logger] %msg");
    el::Loggers::setDefaultConfigurations(config, true);
    return LoggerFactory{};
  }
}
