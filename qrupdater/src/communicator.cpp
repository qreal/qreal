#include "communicator.h"

using namespace qrUpdater;

Communicator::Communicator(QObject *parent)
	: QObject(parent)
{
}

void Communicator::readProgramPath()
{
	int const maxPathSize = 500;
	char buffer[maxPathSize];
	fgets(buffer, maxPathSize, stdin);
	mParentAppPath = QString(buffer);
}

void Communicator::writeQuitMessage()
{
	QString closeMsg("Terminate!\n");
	printf(qPrintable(closeMsg));
	fflush(stdout);
}

void Communicator::writeResumeMessage()
{
	QString resumeMsg("Resume!\n");
	printf(qPrintable(resumeMsg));
	fflush(stdout);
}

void Communicator::writeCustomMessage(QString const &message)
{
	printf(qPrintable(message));
}

QString Communicator::parentAppPath() const
{
	return mParentAppPath;
}


