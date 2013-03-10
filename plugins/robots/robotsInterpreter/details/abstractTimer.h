#pragma once

#include <QtCore/QObject>

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{

class AbstractTimer : public QObject
{
	Q_OBJECT

public:
	virtual void start(int ms) = 0;

signals:
	void timeout();

protected slots:
	void onTimeout();
};

}
}
}
}
