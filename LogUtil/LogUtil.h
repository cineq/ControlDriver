/**
 * @file    LogUtil.h
 * @brief   Utility to log messages during runtime
 * @author  sam grove
 * @version 1.0
 * @see     http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505
 *
 * Copyright (c) 2013
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef LOGUTIL_H
#define LOGUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include "mbed.h"
#include "config.h"

#define STREAM      stdout
#ifdef LOGGING
    #define LOG(...)    \
        fprintf(STREAM, "LOG:   %s L#%d ", __PRETTY_FUNCTION__, __LINE__);  \
        fprintf(STREAM, ##__VA_ARGS__); \
        fflush(STREAM)

    #define WARN(...)   \
        fprintf(STREAM, "WARN:  %s L#%d ", __PRETTY_FUNCTION__, __LINE__);  \
        fprintf(STREAM, ##__VA_ARGS__); \
        fflush(STREAM)
    #define ERROR(...)  \
        fprintf(STREAM, "ERROR: %s L#%d ", __PRETTY_FUNCTION__, __LINE__); \
        fprintf(STREAM, ##__VA_ARGS__); \
        fflush(STREAM); \
        exit(1)
#else
    #define LOG(...) __NOP();
    #define WARN(...) __NOP();
    #define ERROR(...) __NOP();
#endif
 
/** Using the LogUtil class
 *
 * Example:
 * @code
 *  #include "mbed.h"
 *  #include "LogUtil.h"
 *  
 *  LogUtil logger;
 *
 *  int main()
 *  {
 *     LOG("This is a log\n");
 *     WARN("This is a warning\n");
 *
 *     for(int i=0; i<3; ++i) {
 *         LOG("Log message #%d\n", i);
 *     }
 *
 *     for(int i=0; i<3; ++i) {
 *         WARN("Warn message #%d\n", i);
 *     }
 *
 *     ERROR("This is an error\n");
 *  }
 * @endcode
 */
    
/**
 *  @class LogUtil
 *  @brief Different ways to log messages having a standard interface
 */ 
class LogUtil
{
private:
    Serial *_serial;
public:
    
    /** Construct the LogUtil class and configure
     */
    LogUtil(Serial &serial, uint32_t baudrate = 0);
        
};
    

#endif


