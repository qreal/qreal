#pragma once

#include <QtCore/QDir>
#include <QtWidgets/QDialog>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

namespace versioning
{
namespace ui
{

class PushDialog : public QDialog
{
	Q_OBJECT

public:
	PushDialog(QWidget *parent = 0);
	QString url() const;

private:
	QComboBox *mUrlComboBox;
	QLabel *mUrlLabel;
	QDir mCurrentDir;

	QPushButton *createButton(const QString &text, const char *member);
	QComboBox *createComboBox(const QString &text = QString());
};

}
}
