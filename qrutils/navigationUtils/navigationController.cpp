#include "navigationController.h"

using namespace navigation;

NavigationController::NavigationController(NavigationState *state
		, NavigationPageInterface *defaultPage)
	: mDefaultPage(defaultPage), mState(state)
{
}

void NavigationController::switchTo(NavigationPageInterface *page)
{
	emit pageChanged(page);
}

NavigationPageInterface *NavigationController::defaultPage() const
{
	return mDefaultPage;
}

NavigationState *NavigationController::state() const
{
	return mState;
}

void NavigationController::setState(NavigationState *state)
{
	mState = state;
}
