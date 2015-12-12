/* Copyright 2015 CyberTech Labs Ltd.
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

#pragma once

#include <QtWidgets/QComboBox>

#include "utils/utilsDeclSpec.h"

namespace qReal {
namespace ui {

/// Provides a minimalistic COM port picker providing a list of COM-ports automaticly found in system.
/// User though can enter COM port name manually. Entered value will be automaticly stored in settings using the
/// given in constructor key.
class ROBOTS_UTILS_EXPORT ComPortPicker : public QComboBox
{
	Q_OBJECT

public:
	/// @param key The key that will be used to store entered COM port value in SettingsManager.
	explicit ComPortPicker(const QString &key, QObject *parent = 0);

	/// Populates the given combo box items list with the list of known COM ports.
	/// @param settingsKey The name of the key in settings that used to store last selected COM port value. That value
	/// will be setted up as current in \a box.
	static void populate(QComboBox &box, const QString &settingsKey);

private:
	void onPortNameChanged(const QString &newPort);

	const QString mSettingsKey;
};

}
}
