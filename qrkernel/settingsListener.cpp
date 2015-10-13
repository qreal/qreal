/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
