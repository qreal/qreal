#pragma once
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

namespace git
{
namespace ui
{

class BranchNameDialog : public QDialog
{
	Q_OBJECT
public:
	explicit BranchNameDialog(QString const &actionDialogName, QWidget *parent = 0);
	QString getBranchName();

private:
	QLineEdit *branchNameLineEdit;
};

}
}
