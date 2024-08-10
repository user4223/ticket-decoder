#pragma once

#include "LoggerFactory.h"
#include "Logger.h"

#include <easylogging++.h>

#define CREATE_LOGGER(factory) factory.fromPath(__FILE__)

#define LOG_TRACE(logger) CLOG(TRACE, logger.name.c_str())
#define LOG_DEBUG(logger) CLOG(DEBUG, logger.name.c_str())
#define LOG_INFO(logger) CLOG(INFO, logger.name.c_str())
#define LOG_WARN(logger) CLOG(WARNING, logger.name.c_str())
#define LOG_ERROR(logger) CLOG(ERROR, logger.name.c_str())
