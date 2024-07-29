/*
 *  Copyright (c) 2019 Sinric. All rights reserved.
 *  Licensed under Creative Commons Attribution-Share Alike (CC BY-SA)
 *
 *  This file is part of the Sinric Pro (https://github.com/sinricpro/)
 */

#pragma once
#include <stdarg.h>

/* no DEBUG_ESP_PORT.printf( __VA_ARGS__ ) */

void log(const char* format, ...) {
    va_list arg;
    va_start(arg, format);
    size_t ret = vprintf(format, arg);
    va_end(arg);
}

#ifndef NODEBUG_SINRIC
#ifdef DEBUG_ESP_PORT
#define DEBUG_SINRIC(...) log(__VA_ARGS__)
#else
//#define DEBUG_WEBSOCKETS(...) os_printf( __VA_ARGS__ )
#endif
#endif


#ifndef DEBUG_SINRIC
#define DEBUG_SINRIC(...)
#define NODEBUG_SINRIC
#endif