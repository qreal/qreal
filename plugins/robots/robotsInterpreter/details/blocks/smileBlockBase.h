#pragma once

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class SmileBlockBase : public Block
{
public:
	SmileBlockBase(QString const &pathToImage, robotParts::Display &display);

	virtual void run();

private:
	QImage mSmile;
	robotParts::Display &mDisplay;
};

}
}
}
}
}
