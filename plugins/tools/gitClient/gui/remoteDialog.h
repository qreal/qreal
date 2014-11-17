#pragma once

#include <QtCore/QDir>
#include <QtWidgets/QDialog>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

namespace git
{
namespace ui
{

class RemoteDialog : public QDialog
{
	Q_OBJECT

public:
	RemoteDialog(QWidget *parent = 0);
	QString remoteName() const;
	QString remoteUrl() const;

private:
	QComboBox *mRemoteNameComboBox;
	QComboBox *mRemoteUrlComboBox;
	QLabel *mRemoteNameLabel;
	QLabel *mRemoteUrlLabel;
	QDir mCurrentDir;

	QPushButton *createButton(const QString &text, const char *member);
	QComboBox *createComboBox(const QString &text = QString());
};

}
}
