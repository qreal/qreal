#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "block.h"
#include "../robotParts/brick.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class BeepBlock : public Block
{
	Q_OBJECT

public:
	BeepBlock(robotParts::Brick &brick);
	virtual void run();

private slots:
	void timeout();

private:
	robotParts::Brick &mBrick;
	QTimer mTimer;
};

}
}
}
}
}
