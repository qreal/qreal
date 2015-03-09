#include "updateVersionDialog.h"

#include <QtWidgets/QMessageBox>

using namespace updatesChecker;

bool UpdateVersionDialog::promptUpdate(QWidget *parent)
{
	const QString message = QObject::tr("New updates are available!");
	const QString updateButtonText = QObject::tr("Update");
	const QString laterButtonText = QObject::tr("Later");

	return 0 == QMessageBox::information(parent, QObject::tr("Yeah!"), message
			, updateButtonText, laterButtonText, 0, 1);
}
