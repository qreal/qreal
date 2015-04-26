#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>

namespace git
{
namespace ui
{

class CommitDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CommitDialog(QWidget *parent = 0);
	QString message() const;

private:
	QLabel *mEnterMessageLabel;
	QTextEdit *mMessageTextEdit;
	QPushButton *mOkButton;
	QPushButton *mCancel;
};

}
}
