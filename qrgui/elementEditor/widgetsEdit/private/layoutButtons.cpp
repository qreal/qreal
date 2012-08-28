#include "layoutButtons.h"

using namespace Ui::WidgetsEdit;

LayoutButtons::LayoutButtons()
{
	mGridButton = new LayoutButton("Lay out in a grid"
		, QIcon(":/icons/widgetsEditor/layOutInAGrid"), Grid);
	mVerticalButton = new LayoutButton("Lay out vertically"
		, QIcon(":/icons/widgetsEditor/layOutVertically"), Vertical);
	mHorizontalButton = new LayoutButton("Lay out horizontally"
		, QIcon(":/icons/widgetsEditor/layOutHorizontally"), Horizontal);
	mNoLayoutButton = new LayoutButton("Break layout"
		, QIcon(":/icons/widgetsEditor/breakLayout"), NoLayout);

	connect(mGridButton, SIGNAL(clicked(LayoutType))
			, this, SLOT(onLayoutButtonClicked(LayoutType)));
	connect(mVerticalButton, SIGNAL(clicked(LayoutType))
			, this, SLOT(onLayoutButtonClicked(LayoutType)));
	connect(mHorizontalButton, SIGNAL(clicked(LayoutType))
			, this, SLOT(onLayoutButtonClicked(LayoutType)));
	connect(mNoLayoutButton, SIGNAL(clicked(LayoutType))
			, this, SLOT(onLayoutButtonClicked(LayoutType)));

	mButtons << mGridButton << mVerticalButton
		<< mHorizontalButton << mNoLayoutButton;
}

QListIterator<QPushButton *> LayoutButtons::buttonsIterator()
{
	return QListIterator<QPushButton *>(mButtons);
}

void LayoutButtons::disableAllButtons()
{
	setAllButtonsEnabled(false);
}

void LayoutButtons::enableAllButtonsExcept(LayoutType type)
{
	setAllButtonsEnabled();
	buttonFromType(type)->setEnabled(false);
}

void LayoutButtons::onLayoutButtonClicked(LayoutType type)
{
	emit buttonClicked(type);
}

void LayoutButtons::setAllButtonsEnabled(bool enabled)
{
	QListIterator<QPushButton *> iterator = buttonsIterator();
	while (iterator.hasNext()) {
		iterator.next()->setEnabled(enabled);
	}
}

LayoutButton *LayoutButtons::buttonFromType(LayoutType type)
{
	switch(type) {
	case Grid:
		return mGridButton;
	case Vertical:
		return mVerticalButton;
	case Horizontal:
		return mHorizontalButton;
	case NoLayout:
		return mNoLayoutButton;
	default:
		return NULL;
	}
}
