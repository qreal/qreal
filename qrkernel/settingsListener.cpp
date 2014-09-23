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
	qDeleteAll(mListeners);
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

void SettingsListener::disconnect(QObject *object)
{
	for (AbstractListener * const listener : instance().mListeners.values()) {
		if (listener->object() == object) {
			for (QString const &key : instance().mListeners.keys(listener)) {
				instance().mListeners.remove(key, listener);
			}

			delete listener;
		}
	}
}

void SettingsListener::disconnect(QString const &key, QObject *object)
{
	for (AbstractListener * const listener : instance().mListeners.values()) {
		if (listener->object() == object) {
			instance().mListeners.remove(key, listener);
		}

		delete listener;
	}
}
