// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "LoggerFactory.h"
#include "Logger.h"

#include <easylogging++.h>

#define CREATE_LOGGER(factory) factory.fromPath(__FILE__)

#define LOG_TRACE(logger) CLOG(TRACE, logger.getName())
#define LOG_DEBUG(logger) CLOG(DEBUG, logger.getName())
#define LOG_INFO(logger) CLOG(INFO, logger.getName())
#define LOG_WARN(logger) CLOG(WARNING, logger.getName())
#define LOG_ERROR(logger) CLOG(ERROR, logger.getName())
