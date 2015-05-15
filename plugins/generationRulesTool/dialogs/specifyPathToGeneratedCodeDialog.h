#pragma once

#include <QtWidgets/QDialog>
#include <QtCore/QSettings>

#include <qrrepo/logicalRepoApi.h>

namespace Ui {
class SpecifyPathToGeneratedCodeDialog;
}

namespace qReal {
namespace gui {

class SpecifyPathToGeneratedCodeDialog : public QDialog
{
	Q_OBJECT

public:
	SpecifyPathToGeneratedCodeDialog(
			qrRepo::LogicalRepoApi *metamodelRepoApi
			, QWidget *parent = 0);
	~SpecifyPathToGeneratedCodeDialog();

	QString currentPathToFolder() const;
	QString currentFileName() const;

signals:
	void pathsSpecified();

private slots:
	void restoreSettings();

	void saveSettings();

	void specifyFolder();

private:
	Ui::SpecifyPathToGeneratedCodeDialog *mUi;

	qrRepo::LogicalRepoApi *mMetamodelRepoApi;
};

}
}
