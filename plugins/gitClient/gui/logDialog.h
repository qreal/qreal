#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>

namespace git
{
namespace ui
{

class LogDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LogDialog(QWidget *parent = 0);
	void message(QString message);

private:
	QLabel *mEnterMessageLabel;
	QTextEdit *mMessageTextEdit;
};
}

}
