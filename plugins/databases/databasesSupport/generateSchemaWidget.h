#pragma once

#include <QDialog>
#include <QtCore/QTranslator>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>
#include <qrutils/qRealFileDialog.h>
#include <qrkernel/settingsManager.h>

#include <qrgui/preferencesDialog/preferencesPage.h>
#include <../../../qrkernel/settingsManager.h>

namespace Ui {
class GenerateSchemaWidget;
}

namespace qReal {
namespace databasesSupport {

class GenerateSchemaWidget : public QDialog
{
	Q_OBJECT

public:
	explicit GenerateSchemaWidget(QWidget *parent = 0);
	~GenerateSchemaWidget();

	QString const &getDbms();
	QString const &getFilePath();

public slots:
	void updateDbms(QString const &dbms);
	void updateFilePath();


private:
	Ui::GenerateSchemaWidget *mUi;
	QString mDbms;
	QString mFilePath;
};

}
}
