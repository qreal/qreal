#include "settingsListener.h"

#include "qrkernel/settingsManager.h"

using namespace qReal;

SettingsListener::SettingsListener()
{
	connect(SettingsManager::instance(), &SettingsManager::settingsChanged
			, this, &SettingsListener::onSettingsChanged);
}

SettingsListener::~SettingsListener()
{
}

SettingsListener &SettingsListener::instance()
{
	static SettingsListener instance;
	return instance;
}

void SettingsListener::onSettingsChanged(QString const &name, QVariant const &oldValue, QVariant const &newValue)
{
	Q_UNUSED(oldValue)
	for (AbstractListener * const listener : mListeners.values(name)) {
		listener->fireEvent(newValue);
	}
}
