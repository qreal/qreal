#include "communicator.h"

Communicator::Communicator(QObject *parent)
	: QObject(parent)
{
}

void Communicator::readProgramPath()
{
	int const maxPathSize = 300;
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

void Communicator::writeHelpMessage()
{
	QString helpMessage = "usage:\n"
			"updater -unit [unit-name] -version [version] -url [url-to-file] | [-hard]\n"
			"example:\n updater -unit windows -version 0.3.2 -url http://127.0.0.1/updates.xml"
			"or\n"
			"updater -unit cool-module-name -version 2.8a -url http://example.com/updates.xml -hard\n";
	printf(qPrintable(helpMessage));
}

QString Communicator::parentAppPath() const
{
	return mParentAppPath;
}


