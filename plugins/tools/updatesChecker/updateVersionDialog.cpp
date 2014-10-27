#include "updateVersionDialog.h"

#include <QtWidgets/QMessageBox>

using namespace updatesChecker;

bool UpdateVersionDialog::promptUpdate(QWidget *parent)
{
	QString const message = QObject::tr("New updates are available!");
	QString const updateButtonText = QObject::tr("Update");
	QString const laterButtonText = QObject::tr("Later");

	return 0 == QMessageBox::information(parent, QObject::tr("Yeah!"), message
			, updateButtonText, laterButtonText, 0, 1);
}
