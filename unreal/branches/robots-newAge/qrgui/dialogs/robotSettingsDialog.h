#pragma once

#include <QtGui/QDialog>

namespace Ui {
	class RobotSettingsDialog;
}

namespace qReal {
namespace gui {

class RobotSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit RobotSettingsDialog(QWidget *parent = 0);
	~RobotSettingsDialog();

	QString selectedPortName() const;

private slots:
	void ok();
	void cancel();

private:
	Ui::RobotSettingsDialog *mUi;
};

}
}
