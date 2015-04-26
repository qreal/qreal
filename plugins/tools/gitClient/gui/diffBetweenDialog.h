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

class DiffBetweenDialog : public QDialog
{
	Q_OBJECT

public:
	DiffBetweenDialog(QWidget *parent = 0);
	QString firstHash();
	QString secondHash();

private:
	QComboBox *mFirstHash;
	QComboBox *mScndHash;
	QLabel *mFirstHashLabel;
	QLabel *mScndHashLabel;
};

}
}
