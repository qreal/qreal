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

#include <QtCore/QStringList>

#include <kitBase/blocksBase/commonBlocksFactory.h>

namespace ev3 {
namespace blocks {

/// Block factory that creates blocks for EV3 kit.
class Ev3BlocksFactory : public kitBase::blocksBase::CommonBlocksFactory
{
public:
	/// Constructor.
	/// @param interpretedModels - a list of robot ids which are handled by interpreter, so parallel tasks
	///        may be enabled for them.
	Ev3BlocksFactory(const QStringList &interpretedModels);

	qReal::interpretation::Block *produceBlock(const qReal::Id &element) override;
	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;

private:
	const QStringList mInterpretedModels;
};

}
}
