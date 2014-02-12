#include "navigationPageWithMenu.h"

using namespace navigation;

NavigationPageWithMenu::NavigationPageWithMenu()
	: mContent(NULL)
{
}

NavigationMenuContent *NavigationPageWithMenu::menuContent() const
{
	return mContent;
}

void NavigationPageWithMenu::setMenuContent(NavigationMenuContent *content)
{
	mContent = content;
}
