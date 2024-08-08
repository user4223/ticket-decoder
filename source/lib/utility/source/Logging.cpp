
#include "../include/Logging.h"

#include <opencv2/core/utils/logger.hpp>

#include <filesystem>

INITIALIZE_EASYLOGGINGPP;

namespace utility
{
  static auto volatile initialized = false;
  static auto verbose = false;

  Logger LoggerFactory::fromPath(std::string fullPath)
  {
    if (!initialized)
    {
      create(verbose);
    }
    auto name = std::filesystem::path(fullPath).stem().string();
    el::Loggers::getLogger(name);
    return Logger{std::move(name)};
  }

  static auto const disabled = std::string("false");
  static auto const enabled = std::string("true");

  LoggerFactory LoggerFactory::create(bool v)
  {
    verbose = v;
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    auto config = el::Configurations{};
    config.setGlobally(el::ConfigurationType::ToFile, disabled);
    config.setGlobally(el::ConfigurationType::MaxLogFileSize, std::string("0"));
    config.setGlobally(el::ConfigurationType::ToStandardOutput, enabled);
    config.setGlobally(el::ConfigurationType::SubsecondPrecision, std::string("0"));
    config.setGlobally(el::ConfigurationType::PerformanceTracking, disabled);
    config.setGlobally(el::ConfigurationType::LogFlushThreshold, std::string("0"));
    config.setGlobally(el::ConfigurationType::Format, "[%datetime{%Y-%M-%dT%H:%m:%s} %level %logger] %msg");
    config.set(el::Level::Global, el::ConfigurationType::Enabled, disabled);
    config.set(el::Level::Trace, el::ConfigurationType::Enabled, disabled);
    config.set(el::Level::Debug, el::ConfigurationType::Enabled, verbose ? enabled : disabled);
    config.set(el::Level::Info, el::ConfigurationType::Enabled, enabled);
    config.set(el::Level::Warning, el::ConfigurationType::Enabled, enabled);
    config.set(el::Level::Error, el::ConfigurationType::Enabled, enabled);
    config.set(el::Level::Fatal, el::ConfigurationType::Enabled, enabled);
    el::Loggers::setDefaultConfigurations(config, true);

    initialized = true;
    return LoggerFactory{};
  }

  LoggerFactory LoggerFactory::createLazy(bool v)
  {
    verbose = v;
    return LoggerFactory{};
  }
}
