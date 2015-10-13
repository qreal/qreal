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

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include <kitBase/robotModel/robotModelInterface.h>
#include "blocksFactoryManagerInterface.h"

namespace interpreterCore {

/// Implementation of blocks factory manager that can provide information about blocks supported by given kit and given
/// model, and create blocks as required by interpreter.
class BlocksFactoryManager : public BlocksFactoryManagerInterface
{
public:
	~BlocksFactoryManager() override;

	void addFactory(kitBase::blocksBase::BlocksFactoryInterface * const factory
			, const kitBase::robotModel::RobotModelInterface *robotModel = nullptr) override;

	qReal::interpretation::BlockInterface *block(const qReal::Id &element
			, const kitBase::robotModel::RobotModelInterface &robotModel) override;

	QSet<qReal::Id> enabledBlocks(const kitBase::robotModel::RobotModelInterface &robotModel) const override;

	QSet<qReal::Id> visibleBlocks(const kitBase::robotModel::RobotModelInterface &robotModel) const override;

	QSet<qReal::Id> commonBlocks() const override;

private:
	/// Provides a list of factories that can create blocks for given robot model (common factory that creates blocks
	/// for all models, and model-specific factories).
	QList<kitBase::blocksBase::BlocksFactoryInterface *> factoriesFor(
			const kitBase::robotModel::RobotModelInterface &robotModel) const;

	/// Maps robot model to a set of block factories that can provide blocks for it.
	// Has ownership over factories.
	// Does not have ownership over robot models.
	QMap<const kitBase::robotModel::RobotModelInterface *
			, kitBase::blocksBase::BlocksFactoryInterface *> mFactories;

};

}
