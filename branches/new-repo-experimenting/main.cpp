#include <QDebug>
#include <QApplication>

#include "mainwindow.h"

#include "realreporoles.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	return app.exec();

	qDebug() << "Class"
		<< SQLFields::Class[ Unreal::FieldsRole - Unreal::UserRole - 1 ];
	qDebug()	<< SQLFields::Class[ 666 ];

	return 0;
}
