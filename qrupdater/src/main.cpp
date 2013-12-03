#include <QtWidgets/QApplication>

#include "updateProcessor.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	qrUpdater::UpdateProcessor updater;
	updater.startUpdateControl();

	return app.exec();
}
