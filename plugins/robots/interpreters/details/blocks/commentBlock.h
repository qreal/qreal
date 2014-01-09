#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class CommentBlock : public Block
{
	Q_OBJECT

public:
	CommentBlock();

public:
	virtual void run();

private:
	virtual bool initNextBlocks();
};

}
}
}
}
}
