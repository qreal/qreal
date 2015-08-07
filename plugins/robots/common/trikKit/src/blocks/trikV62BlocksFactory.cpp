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

#include "trikKit/blocks/trikV62BlocksFactory.h"

#include <kitBase/blocksBase/common/clearEncoderBlock.h>
#include <kitBase/blocksBase/common/waitForEncoderBlock.h>

#include "details/trikEnginesBackwardBlock.h"
#include "details/trikEnginesForwardBlock.h"

using namespace trik::blocks;
using namespace trik::blocks::details;
using namespace kitBase::blocksBase::common;

qReal::interpretation::Block *TrikV62BlocksFactory::produceBlock(const qReal::Id &element)
{
	if (elementMetatypeIs(element, "TrikV62AngularServo")) {
		// AngularServo and EnginesForward are synonyms since angular and radial servos are controlled the same way.
		return new details::TrikEnginesForwardBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikV62ClearEncoder")) {
		return new ClearEncoderBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "TrikV62WaitForEncoder")) {
		return new WaitForEncoderBlock(mRobotModelManager->model());
	}

	return TrikBlocksFactoryBase::produceBlock(element);
}

qReal::IdList TrikV62BlocksFactory::providedBlocks() const
{
	qReal::IdList result;

	result << TrikBlocksFactoryBase::providedBlocks();

	result
			<< id("TrikV62AngularServo")
			<< id("TrikV62ClearEncoder")
			<< id("TrikV62WaitForEncoder")
	;

	return result;
}
