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

#include <qrkernel/ids.h>
#include <qrtext/languageToolboxInterface.h>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrutils/interpreter/blockInterface.h>

#include <kitBase/robotModel/robotModelManagerInterface.h>

namespace kitBase {
namespace blocksBase {

/// An inteface of the factory that produces blocks for interpreter.
class BlocksFactoryInterface
{
public:
	virtual ~BlocksFactoryInterface() {}

	/// Creates a new block by given id.
	/// Transfers block ownership to caller.
	virtual qReal::interpretation::BlockInterface *block(const qReal::Id &element) = 0;

	/// Returns a list of blocks that can be instantiated by this factory.
	virtual qReal::IdList providedBlocks() const = 0;

	/// Returns a list of blocks that will be disabled in palette when corresponding to this factory robot model
	/// is selected.
	virtual qReal::IdList blocksToDisable() const = 0;

	/// Initializes this factory with external compoments. Those components are used for correct blocks initialization.
	virtual void configure(const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, const qReal::LogicalModelAssistInterface &logicalModelApi
			, kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qReal::ErrorReporterInterface &errorReporter
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			) = 0;
};

}
}
