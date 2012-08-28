#pragma once

#include <QtCore/QList>

#include "layoutButton.h"

namespace Ui
{
namespace WidgetsEdit
{

class LayoutButtons : public QObject
{
	Q_OBJECT

public:
	LayoutButtons();
	QListIterator<QPushButton *> buttonsIterator();

	void disableAllButtons();
	void enableAllButtonsExcept(LayoutType type);

signals:
	void buttonClicked(LayoutType type);

private slots:
	void onLayoutButtonClicked(LayoutType type);
	void setAllButtonsEnabled(bool enabled = true);
	LayoutButton *buttonFromType(LayoutType type);

private:
	LayoutButton *mGridButton;
	LayoutButton *mVerticalButton;
	LayoutButton *mHorizontalButton;
	LayoutButton *mNoLayoutButton;

	QList<QPushButton *> mButtons;

};

}
}
