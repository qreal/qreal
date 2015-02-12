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

void SettingsListener::onSettingsChanged(const QString &name, const QVariant &oldValue, const QVariant &newValue)
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
			for (const QString &key : instance().mListeners.keys(listener)) {
				instance().mListeners.remove(key, listener);
			}

			delete listener;
		}
	}
}

void SettingsListener::disconnect(const QString &key, QObject *object)
{
	for (AbstractListener * const listener : instance().mListeners.values()) {
		if (listener->object() == object) {
			instance().mListeners.remove(key, listener);
		}

		delete listener;
	}
}
