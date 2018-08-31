/* Copyright 2017 QReal Research Group
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

#include "pioneerKit/blocks/pioneerBlocksFactory.h"

using namespace pioneer::blocks;

qReal::interpretation::Block *PioneerBlocksFactory::produceBlock(const qReal::Id &element)
{
	Q_UNUSED(element);
	return nullptr;
}

qReal::IdList PioneerBlocksFactory::providedBlocks() const
{
	return {
			id("GeoTakeoff")
			, id("GeoLanding")
			, id("GoToPoint")
			, id("GoToGPSPoint")
			, id("PioneerGetLPSPosition")
			, id("PioneerPrint")
			, id("PioneerSystem")
			, id("PioneerLed")
			, id("PioneerReadRangeSensor")
			, id("PioneerMagnet")
			, id("PioneerYaw")
	};
}

qReal::IdList PioneerBlocksFactory::blocksToDisable() const
{
	return {
	};
}

qReal::IdList PioneerBlocksFactory::blocksToHide() const
{
	return {
			id("Function")
			, id("SwitchBlock")
			, id("Loop")
			, id("Subprogram")
			, id("Fork")
			, id("Join")
			, id("KillThread")
//			, id("IfBlock")
//			, id("FiBlock")

			, id("SendMessageThreads")

			, id("ReceiveMessageThreads")

			, id("PrintText")
			, id("ClearScreen")
			, id("MarkerDown")
			, id("MarkerUp")
	};
}
