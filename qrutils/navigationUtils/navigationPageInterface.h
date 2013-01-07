#pragma once

#include "../utilsDeclSpec.h"
#include "navigationState.h"

namespace navigation
{

/// An interface to be implemented by each page used by navigation framework
class QRUTILS_EXPORT NavigationPageInterface
{
public:
	/// Called each time when page was shown. Default implementation does nothing.
	virtual void onShown(NavigationState *state)
	{
		Q_UNUSED(state)
	}

	/// Called each time when page was hidden. Default implementation does nothing.
	virtual void onHidden()
	{
	}
};

}
