/* Copyright 2016 CyberTech Labs Ltd.
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
 * limitations under the License. */

#pragma once

#include "declSpec.h"

namespace qrTest {

/// Utility class that provides event loop based waiting.
class TEST_UTILS_EXPORT Wait
{
public:
	/// Wait for given amount of milliseconds. Launches event loop so thread is not blocked and can still process
	/// messages.
	static TEST_UTILS_EXPORT void wait(int msecs);
};

}
