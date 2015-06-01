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

#include <qrutils/interpreter/block.h>
#include <kitBase/robotModel/robotModelInterface.h>

namespace utils {
class AbstractTimer;
}

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for timer block.
/// Waits for the given amount of time using robot model`s timeline.
/// @todo Why it does not inherit WaitBlock?
class TimerBlock : public qReal::interpretation::Block
{
	Q_OBJECT

public:
	explicit TimerBlock(kitBase::robotModel::RobotModelInterface &robotModel);
	~TimerBlock() override;

	void run() override;

private slots:
	void timeout();

private:
	utils::AbstractTimer * const mTimer;  // Has ownership (via Qt parent-child system).
};

}
}
}
