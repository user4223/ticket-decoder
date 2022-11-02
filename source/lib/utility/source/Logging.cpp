
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
    el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);

    auto config = el::Configurations{};
    auto const disabled = std::string("false");
    auto const enabled = std::string("true");
    config.setGlobally(el::ConfigurationType::ToFile, disabled);
    config.setGlobally(el::ConfigurationType::MaxLogFileSize, std::string("0"));
    config.setGlobally(el::ConfigurationType::ToStandardOutput, enabled);
    config.setGlobally(el::ConfigurationType::SubsecondPrecision, std::string("0"));
    config.setGlobally(el::ConfigurationType::PerformanceTracking, disabled);
    config.setGlobally(el::ConfigurationType::LogFlushThreshold, std::string("0"));
    config.setGlobally(el::ConfigurationType::Format, "[%level %datetime{%Y-%M-%dT%H:%m:%s} %logger] %msg");
    config.set(el::Level::Trace, el::ConfigurationType::Enabled, disabled);
    config.set(el::Level::Debug, el::ConfigurationType::Enabled, disabled);
    config.set(el::Level::Info, el::ConfigurationType::Enabled, enabled);
    config.set(el::Level::Warning, el::ConfigurationType::Enabled, enabled);
    config.set(el::Level::Error, el::ConfigurationType::Enabled, enabled);
    config.set(el::Level::Fatal, el::ConfigurationType::Enabled, enabled);
    el::Loggers::setDefaultConfigurations(config, true);

    return LoggerFactory{};
  }
}
