/* Copyright 2015 CyberTech Labs Ltd.
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

#include <kitBase/blocksBase/robotsBlock.h>

namespace kitBase {

namespace robotModel {
namespace robotParts {
class Random;
}
}

namespace blocksBase {
namespace common {

/// Implementation for "random init" block that uses random device provided by robot model (instead of library C++
/// implementation in a block with the same name from qrutils::interpreter).
class ROBOTS_KIT_BASE_EXPORT RandomInitBlock : public RobotsBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit RandomInitBlock(robotModel::RobotModelInterface &robotModel);

	void run() override;

private:
	robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
