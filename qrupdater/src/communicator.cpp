#include "communicator.h"

using namespace qrUpdater;

Communicator::Communicator(QObject *parent)
	: QObject(parent)
{
}

void Communicator::writeMessage(QString const &message)
{
	printf(qPrintable(message));
}
