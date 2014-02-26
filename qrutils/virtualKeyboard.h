#pragma once

#include "utilsDeclSpec.h"

namespace utils {

/// On-screen control api keyboard
class QRUTILS_EXPORT VirtualKeyboard
{
public:
	/// Shows on-screen keyboard. Implementation is different for each platform.
	/// @warning Currently works only in windows 8
	static void show();

private:
	static void showWindows8Keyboard();
};

}
