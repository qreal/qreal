#pragma once

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class SetBackgroundBlock : public Block
{
public:
	explicit SetBackgroundBlock(robotParts::Display &display);

	virtual void run();

private:
	QColor propertyToColor(QString const &property) const;

	robotParts::Display &mDisplay;
};

}
}
}
}
}
