#pragma once

#include <qrkernel/ids.h>
#include <qrkernel/definitions.h>

namespace qReal {

class HintInterface {
public:
	virtual void addHint(QString const &message, Id const &position = Id::rootId()) = 0;
};

}
