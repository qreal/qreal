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

#pragma once

#include <QtCore/QObject>

#include <qrutils/interpreter/stopReason.h>
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {

/// Events provided to kit plugins by interpreter.
class ROBOTS_KIT_BASE_EXPORT EventsForKitPluginInterface : public QObject
{
	Q_OBJECT

signals:
	/// Program interpretation started.
	void interpretationStarted();

	/// Program interpretation finished or stopped by user.
	/// @param reason The reason why the interpretation stopped.
	void interpretationStopped(qReal::interpretation::StopReason reason);

	/// Robot model is changed in settings (or selected in a toolbar).
	void robotModelChanged(const QString &newModelName);
};

}
