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

class RemoteDialog : public QDialog
{
	Q_OBJECT

public:
	RemoteDialog(QWidget *parent = 0);
	QString url() const;
	QString target() const;

private:
	QComboBox *mUrlComboBox;
	QComboBox *mPathComboBox;
	QLabel *mUrlLabel;
	QLabel *mPathLabel;
	QDir mCurrentDir;

	QPushButton *createButton(const QString &text, const char *member);
	QComboBox *createComboBox(const QString &text = QString());
};

}
}
