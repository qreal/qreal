#pragma once

#include <qrutils/interpreter/block.h>

namespace qReal {
namespace interpretation {
namespace blocks {

class ReceiveThreadMessageBlock : public Block
{
	Q_OBJECT
public:
	void run() override;

	void receiveMessage(const QString &message);

private:
	QString mVariable;
};

}
}
}
