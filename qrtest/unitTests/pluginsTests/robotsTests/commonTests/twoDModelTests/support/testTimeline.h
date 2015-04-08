/* Copyright 2007-2015 QReal Research Group
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

#include <utils/timelineInterface.h>

namespace qrTest {
namespace robotsTests {
namespace commonTwoDModelTests {

/// A helper class with managed timestamp switching.
class TestTimeline : public utils::TimelineInterface
{
public:
	quint64 timestamp() const override;
	utils::AbstractTimer *produceTimer() override;

	/// Sets current timestamp value; all subsequent timestamp() calls will return
	/// the given value till the other setTimestamp() call.
	void setTimestamp(quint64 value);

private:
	quint64 mTimestamp = 0;
};

}
}
}
