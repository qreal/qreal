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

#include "dummyBlock.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace qReal;

void DummyBlock::run()
{
	emit done(mNextBlockId);
}

bool DummyBlock::initNextBlocks()
{
	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	if (links.count() == 0) {
		mNextBlockId = Id();
	} else {
		mNextBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(links[0], id());
	}

	return true;
}
