/* Copyright 2014-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
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

namespace qReal {
namespace gui {

class HintReporter;
class Arrow;
class ScriptAPI;

/// This class provides scripting access to hinting features in system API. Allows to show beautiful bubbling messages
/// and arrows pointing given widgets.
class HintAPI : public QObject
{
	Q_OBJECT

public:
	HintAPI();

	/// Sends beautifully bubble-like message appearing in parent widget existing for \a lifeTime
	/// (conidering fade-away effect).
	Q_INVOKABLE void addHint(const QString &message, int lifeTime, QWidget *parent);

	/// Draws arrow pointing to \a target widget with the given tilt \a angle. Exists for lifeTime
	/// (considering fade-away effect).
	Q_INVOKABLE void arrowToWidget(QWidget *target, qreal angle, int lifeTime, QWidget *parent);
};

}
}
