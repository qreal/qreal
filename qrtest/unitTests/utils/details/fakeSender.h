#pragma once

#include <QtCore/QObject>

namespace qrTest {
namespace details {

/// Helper class that relays incoming signals. Needed to simulate different sender() for QSignalMapper when we want to
/// map different signals of the same object. See http://www.qtcentre.org/threads/39596-QSignalMapper-One-single-Object
/// for discussion of a similar task and https://github.com/minirop/SignalsSpy for similar solution.
class FakeSender : public QObject
{
	Q_OBJECT

signals:
	/// A signal that is emitted when object receives something on a "receive()" slot.
	void send();

public slots:
	/// A slot that triggers send() signal, used to relay signal with this object as a sender.
	void receive()
	{
		emit send();
	}
};

}
}
