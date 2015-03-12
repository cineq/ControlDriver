/**
 * @file    LogUtil.cpp
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
 
 #include "LogUtil.h"
 #include "mbed.h"
 
LogUtil::LogUtil(Serial &serial, uint32_t baudrate)
{
    _serial = &serial;
    (baudrate > 0) ? _serial->baud(baudrate) : __NOP();
    _serial->printf("\033[2J");  // clear the terminal
    _serial->printf("\033[1;1H");// and set the cursor to home
    wait(0.5f);
    return;
}
     
 
 
