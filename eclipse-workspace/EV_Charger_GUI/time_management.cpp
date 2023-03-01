/*
 * Name : time_management.cpp
 * Author : Microchip MPU32 Technical marketing
 *
 * Copyright (C) Microchip Technology Inc.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string>
#include <ctime>
#include <memory>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "time_management.h"


std::string current_time()
{
	const auto now = std::chrono::system_clock::now();
	const auto in_time_t = std::chrono::system_clock::to_time_t(now);
	const auto local = std::localtime(&in_time_t);
	std::stringstream ss;
	ss << std::put_time(local, "%I:%M:%S %p");
	auto result = ss.str();
	if (!result.empty() && result[0] == '0')
		result.erase(0, 1);
	return result;
}

std::string current_date()
{
    const auto now = std::chrono::system_clock::now();
    const auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%A, %B %e");
    return ss.str();
}

