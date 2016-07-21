/* Copyright 2016 Yurii Litvinov, CyberTech Labs Ltd.
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

namespace trik {
namespace robotCommunication {

/// Helper class that basically emits signal when asked to do so. Used in state machines to implement conditional
/// transition (recommended way is to subclass QSignalTransition, but signal generator and lambda is actually smaller
/// and simplier in our case).
class GuardSignalGenerator : public QObject
{
	Q_OBJECT

signals:
	/// Emitted when somebody calls onTrigger() slot.
	void guardSatisfied();

public slots:
	/// Emit guardSatisfied() signal.
	void onTrigger()
	{
		emit guardSatisfied();
	}
};

}
}
