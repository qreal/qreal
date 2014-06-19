#pragma once

#include <QtCore/QObject>

#include "../utilsDeclSpec.h"

namespace navigation
{

/// Just a dumb container for passing user data
class QRUTILS_EXPORT NavigationState
{
public:
	NavigationState() {}

	template<typename T>
	T state() const { return (T)mState; }
	template<typename T>
	void setState(T const &state) { mState = state; }

private:
	const void *mState;
};

}
