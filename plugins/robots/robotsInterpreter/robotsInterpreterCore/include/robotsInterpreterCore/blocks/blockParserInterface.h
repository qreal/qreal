#pragma once

#include <qrkernel/ids.h>
#include <qrutils/expressionsParser/number.h>

namespace robotsInterpreterCore {
namespace blocks {

class BlockParserInterface
{
public:
	virtual ~BlockParserInterface() {}

	virtual utils::Number standartBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId) = 0;
	virtual void functionBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId) = 0;
	virtual void deselect() = 0;
};

}
}
