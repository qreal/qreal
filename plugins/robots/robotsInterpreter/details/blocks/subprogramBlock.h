#pragma once

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class SubprogramBlock : public Block
{
	Q_OBJECT

public:
	SubprogramBlock();

	virtual void run();
	virtual void finishedSteppingInto();
};

}
}
}
}
}
