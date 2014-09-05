#pragma once

#include <qrkernel/ids.h>
#include <qrtext/lua/luaToolbox.h>

#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <interpreterBase/blocksBase/blockInterface.h>
#include <interpreterBase/blocksBase/blockParserInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace interpreterBase {
namespace blocksBase {

/// An inteface of the factory that produces blocks for interpreter.
class BlocksFactoryInterface
{
public:
	virtual ~BlocksFactoryInterface() {}

	/// Creates a new block by given id.
	/// Transfers block ownership to caller.
	virtual BlockInterface *block(qReal::Id const &element) = 0;

	/// Returns a list of blocks that can be instantiated by this factory.
	virtual qReal::IdList providedBlocks() const = 0;

	/// Returns a list of blocks that will be disabled in palette when corresponding to this factory robot model
	/// is selected.
	virtual qReal::IdList blocksToDisable() const = 0;

	/// Initializes this factory with external compoments. Those components are used for correct blocks initialization.
	virtual void configure(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qReal::ErrorReporterInterface &errorReporter
			, qrtext::lua::LuaToolbox &newParser
			) = 0;
};

}
}
