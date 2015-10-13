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

#include "blocksFactoryInterface.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace qReal {
namespace interpretation {
class Block;
}
}

namespace kitBase {
namespace blocksBase {

/// Base implementation of all blocks factories. Provides common functionality and default implementations for
/// BlocksFactoryInterface methods.
class ROBOTS_KIT_BASE_EXPORT CommonBlocksFactory : public BlocksFactoryInterface
{
public:
	CommonBlocksFactory();

	qReal::interpretation::BlockInterface *block(const qReal::Id &element) override;

	void configure(const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, const qReal::LogicalModelAssistInterface &logicalModelApi
			, kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qReal::ErrorReporterInterface &errorReporter
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			) final;

	/// May be overrided to disable some blocks from palette when binded to this factory
	/// robot model is active. Default implementation disables nothing.
	qReal::IdList blocksToDisable() const override;

protected:
	static qReal::Id id(const QString &metatype);
	static bool elementMetatypeIs(const qReal::Id &element, const QString &metatype);

	/// Implementation must instantiate a concrete block object by the given id. Future preparation
	/// is performed in parent code.
	virtual qReal::interpretation::Block *produceBlock(const qReal::Id &element) = 0;

	/// @todo: there is no such thing as protected fields. One can not trust its descendants.
	kitBase::robotModel::RobotModelManagerInterface *mRobotModelManager;  // Does not have ownership.
	const qReal::GraphicalModelAssistInterface *mGraphicalModelApi;  // Does not have ownership.
	const qReal::LogicalModelAssistInterface *mLogicalModelApi;  // Does not have ownership.
	qReal::ErrorReporterInterface *mErrorReporter;  // Does not have ownership.
	qrtext::LanguageToolboxInterface *mParser;  // Does not have ownership.
};

}
}
