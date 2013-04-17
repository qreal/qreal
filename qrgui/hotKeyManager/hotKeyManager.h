#pragma once;

namespace qReal {

class HotKeyManager
{
public:
	static HotKeyManager* instance();
private:
	HotKeyManager();

	static HotKeyManager* mInstance;
};

}
