#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class DummyBlock : public Block
{
	Q_OBJECT

public:
	virtual void run();

private slots:
	void timeout();

private:
	QTimer mTimer;
};

}
}
}
}
}
