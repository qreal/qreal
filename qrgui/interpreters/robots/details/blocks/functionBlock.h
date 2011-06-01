#pragma once

#include <QtCore/QObject>

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class FunctionBlock : public Block
{
	Q_OBJECT

public:
	FunctionBlock();
	virtual void run();
};

}
}
}
}
}
