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

class CheckoutDialog : public QDialog
{
	Q_OBJECT

public:
	CheckoutDialog(QWidget *parent = 0);
	QString url() const;
	QString target() const;

public slots:
	void browse();

private:
	QComboBox *mUrlComboBox;
	QComboBox *mPathComboBox;
	QLabel *mUrlLabel;
	QLabel *mPathLabel;
	QPushButton *mBrowseButton;
	QDir mCurrentDir;

	QPushButton *createButton(const QString &text, const char *member);
	QComboBox *createComboBox(const QString &text = QString());
};

}
}
