#include "generateSchemaWidget.h"
#include "ui_generateSchemaWidget.h"

namespace qReal {
namespace databasesSupport {

GenerateSchemaWidget::GenerateSchemaWidget(QWidget *parent) :
	QDialog(parent)
	, mUi(new Ui::GenerateSchemaWidget)
	, mDbms(QString("SQLite"))
	, mFilePath(QString(""))
{
	mUi->setupUi(this);
	this->setVisible(false);
	connect(mUi->dbmsBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateDbms(QString)));
	connect(mUi->reverseEngineerFileButton, SIGNAL(clicked()), this, SLOT(updateFilePath()));
}

GenerateSchemaWidget::~GenerateSchemaWidget()
{
	delete mUi;
}

QString const &GenerateSchemaWidget::getDbms()
{
	if (mDbms == "SQLite")
		return QString("Sqlite");
	else
		return QString("MicrosoftAccess");
}

QString const &GenerateSchemaWidget::getFilePath()
{
	return mFilePath;
}

void GenerateSchemaWidget::updateDbms(QString const &dbms)
{
	mDbms = dbms;
}

void GenerateSchemaWidget::updateFilePath()
{
	const QString path = utils::QRealFileDialog::getOpenFileName("FileForReverseEngineer"
			,this, tr("Open Directory")).replace("\\", "/");
	mUi->reverseEngineerFilePath->setText(path);
	mFilePath = path;
}

}
}
