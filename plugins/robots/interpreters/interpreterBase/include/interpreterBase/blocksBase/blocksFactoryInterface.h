#pragma once

#include <interpreterBase/blocksBase/blockInterface.h>
#include <interpreterBase/blocksBase/blockParserInterface.h>
#include <qrkernel/ids.h>

namespace interpreterBase {
namespace blocksBase {

class BlocksFactoryInterface
{
public:
	virtual ~BlocksFactoryInterface() {}

	/// @todo Unneeded, we can supply a parser to a block when we execute it.
	virtual void setParser(BlockParserInterface * const parser) = 0;

	/// Transfers block ownership to caller.
	virtual BlockInterface *block(qReal::Id const &element) = 0;
	virtual qReal::IdList providedBlocks() const = 0;

protected:
	/// @todo: other place for it?
	bool elementMetatypeIs(qReal::Id const &element, QString const &metatype)
	{
		return element.type() == qReal::Id("RobotsMetamodel", "RobotsDiagram", metatype);
	}
};

}
}
