
#include "../include/Logging.h"

#include <opencv2/core/utils/logger.hpp>

#include <filesystem>

INITIALIZE_EASYLOGGINGPP;

namespace utility
{
  static auto volatile initialized = false;

  struct Config
  {
    bool verbose = false;
    bool toLogFile = false;
  };

  auto config = Config{};

  Logger LoggerFactory::fromPath(std::string fullPath)
  {
    if (!initialized)
    {
      create(config.verbose, config.toLogFile);
    }
    auto name = std::filesystem::path(fullPath).stem().string();
    el::Loggers::getLogger(name);
    return Logger{std::move(name)};
  }

  static auto const disabled = std::string("false");
  static auto const enabled = std::string("true");

  LoggerFactory LoggerFactory::create(bool verbose, bool toLogFile)
  {
    config = Config{verbose, toLogFile};

    auto configuration = el::Configurations{};
    configuration.setGlobally(el::ConfigurationType::ToFile, config.toLogFile ? enabled : disabled);
    configuration.setGlobally(el::ConfigurationType::Filename, "ticketdecoder.log");
    configuration.setGlobally(el::ConfigurationType::MaxLogFileSize, std::to_string(1 * 1024 * 1024));

    configuration.setGlobally(el::ConfigurationType::ToStandardOutput, config.toLogFile ? disabled : enabled);

    configuration.setGlobally(el::ConfigurationType::SubsecondPrecision, std::string("0"));
    configuration.setGlobally(el::ConfigurationType::PerformanceTracking, disabled);
    configuration.setGlobally(el::ConfigurationType::LogFlushThreshold, std::to_string(10));
    configuration.setGlobally(el::ConfigurationType::Format, "[%datetime{%Y-%M-%dT%H:%m:%s} %level %logger] %msg");

    configuration.set(el::Level::Global, el::ConfigurationType::Enabled, disabled);
    configuration.set(el::Level::Trace, el::ConfigurationType::Enabled, disabled);
    configuration.set(el::Level::Debug, el::ConfigurationType::Enabled, verbose ? enabled : disabled);
    configuration.set(el::Level::Info, el::ConfigurationType::Enabled, enabled);
    configuration.set(el::Level::Warning, el::ConfigurationType::Enabled, enabled);
    configuration.set(el::Level::Error, el::ConfigurationType::Enabled, enabled);
    configuration.set(el::Level::Fatal, el::ConfigurationType::Enabled, enabled);
    el::Loggers::setDefaultConfigurations(configuration, true);

    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    initialized = true;
    return LoggerFactory{};
  }

  LoggerFactory LoggerFactory::createLazy(bool verbose, bool toLogFile)
  {
    config = Config{verbose, toLogFile};
    return LoggerFactory{};
  }
}
