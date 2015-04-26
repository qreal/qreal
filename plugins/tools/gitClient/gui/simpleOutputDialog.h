#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>

namespace git
{
namespace ui
{

class SimpleOutputDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SimpleOutputDialog(QString const &outputDialogName, QWidget *parent = 0);
	void message(QString message);

private:
	QTextBrowser *mMessageText;
};
}

}
