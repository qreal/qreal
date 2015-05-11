#pragma once

#include <QtWidgets/QDialog>
#include <QtCore/QSettings>

namespace Ui {
class SpecifyPathToGeneratedCodeDialog;
}

namespace qReal {
namespace gui {

class SpecifyPathToGeneratedCodeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SpecifyPathToGeneratedCodeDialog(QWidget *parent = 0);
	~SpecifyPathToGeneratedCodeDialog();

	void restoreSettings();
	void saveSettings();

	QString currentPathToFolder() const;
	QString currentFileName() const;

private:
	Ui::SpecifyPathToGeneratedCodeDialog *mUi;
};

}
}
