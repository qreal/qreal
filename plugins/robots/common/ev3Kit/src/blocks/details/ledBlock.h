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

#include "kitBase/blocksBase/common/deviceBlock.h"
#include "ev3Kit/robotModel/parts/ev3Led.h"

namespace ev3 {
namespace blocks {
namespace details {

/// An interpreter`s implementation for LED block, sets LED color.
class LedBlock : public kitBase::blocksBase::common::DeviceBlock<robotModel::parts::Ev3Led>
{
	Q_OBJECT

public:
	explicit LedBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::Ev3Led &led) override;
	robotModel::parts::Ev3LedColor toLedColor(const QString &color) const;
};

}
}
}
