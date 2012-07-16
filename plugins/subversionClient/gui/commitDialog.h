#pragma once

#include <QtGui>

namespace versioning
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
