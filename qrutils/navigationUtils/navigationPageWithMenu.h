#pragma once

#include "../utilsDeclSpec.h"
#include "navigationPageInterface.h"
#include "navigationMenu.h"

namespace navigation
{

/// Appends navigation page with menu specification possibility
class QRUTILS_EXPORT NavigationPageWithMenu : public NavigationPageInterface
{
public:
	NavigationPageWithMenu();

	NavigationMenuContent *menuContent() const;

protected:
	virtual void setMenuContent(NavigationMenuContent *content);

private:
	NavigationMenuContent *mContent;
};

}
