#include <QtCore/QCoreApplication>

#include "updateProcessor.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	qrUpdater::UpdateProcessor updater;
	updater.startUpdateControl();

	return app.exec();
}
