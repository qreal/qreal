#include "navigationMenu.h"

using namespace navigation;

NavigationMenuContent::NavigationMenuContent()
	: mLayout(new QBoxLayout(QBoxLayout::LeftToRight))
{
	mLayout->setMargin(0);
	mLayout->setSpacing(0);
}

NavigationMenuContent *NavigationMenuContent::clone()
{
	NavigationMenuContent *other = new NavigationMenuContent;
	for (int i = 0; i < this->mLayout->count(); ++i) {
		other->mLayout->addItem(this->mLayout->itemAt(i));
	}
	return other;
}

void NavigationMenuContent::appendButton(QPushButton *button)
{
	mLayout->addWidget(button);
}

void NavigationMenuContent::appendButtons(QList<QPushButton *> const &buttons)
{
	foreach (QPushButton *button, buttons) {
		mLayout->addWidget(button);
	}
}

void NavigationMenuContent::insertButton(QPushButton *button, int index)
{
	mLayout->insertWidget(index, button);
}

void NavigationMenuContent::insertButtons(QList<QPushButton *> const &buttons, int index)
{
	foreach (QPushButton *button, buttons) {
		mLayout->insertWidget(index++, button);
	}
}

void NavigationMenuContent::addStretch()
{
	mLayout->addStretch();
}

void NavigationMenuContent::insertStretch(int index)
{
	mLayout->insertStretch(index);
}

QBoxLayout *NavigationMenuContent::toLayout()
{
	return mLayout;
}

NavigationMenu::NavigationMenu()
	: mLayout(new QStackedLayout)
{
	setFrameShape(StyledPanel);
	setFrameShadow(Raised);
	setLayout(mLayout);
}

void NavigationMenu::setContent(NavigationMenuContent *content)
{
	if (!content) {
		hide();
		return;
	}
	show();
	QLayout *contentLayout = content->toLayout();
	int index = indexOf(contentLayout);
	if (index < 0) {
		QWidget *newWidget = new QWidget;
		newWidget->setLayout(contentLayout);
		index = mLayout->addWidget(newWidget);
	}
	mLayout->setCurrentIndex(index);
}

int NavigationMenu::indexOf(QLayout *layout)
{
	for (int i = 0; i < mLayout->count(); ++i) {
		if (mLayout->widget(i)->layout() == layout) {
			return i;
		}
	}
	return -1;
}
