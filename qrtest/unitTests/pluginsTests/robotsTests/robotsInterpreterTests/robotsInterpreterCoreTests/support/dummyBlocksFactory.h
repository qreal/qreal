#pragma once

#include "src/blocks/blockFactoryInterface.h"

namespace qrTest {
namespace robotsInterpreterCoreTests {

class DummyBlockFactory : public robotsInterpreterCore::blocks::BlocksFactoryInterface
{
public:
	void setParser(robotsInterpreterCore::blocks::BlockParserInterface * const parser);
	robotsInterpreterCore::blocks::BlockInterface *block(qReal::Id const &element);
	qReal::IdList providedBlocks() const;
};

}
}
