#pragma once

#include <QtCore/QObject>

namespace qrTest {

class FakeSender : public QObject
{
	Q_OBJECT

signals:
	void send();

public slots:
	void receive()
	{
		emit send();
	}
};

}
