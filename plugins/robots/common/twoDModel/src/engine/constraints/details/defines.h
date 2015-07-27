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

#include <functional>
#include <QtCore/QMap>
#include <QtCore/QVariant>

namespace twoDModel {
namespace constraints {
namespace details {

class Event;

/// A helper object for reporting constraints violation or passing.
class StatusReporter : public QObject
{
	Q_OBJECT

signals:
	/// Emitted when <success> tag triggered.
	/// @param deferred If true then the program execution will not be stopped immediately.
	/// Instead the success trigger will be emitted just when program execution is stopped.
	/// However this will not be done if during the period between deferred success event and
	/// program finish fail event will be fired. Then the program will just be considered failed.
	void success(bool deferred);

	/// Emitted when <fail> tag triggered.
	void fail(const QString &message);

	/// Emitted when checker program written incorrectly with the reason as parameter.
	void checkerError(const QString &message);
};

/// Represents logical operations that can be used for concatenating conditions.
enum class Glue
{
	/// Logical 'and'
	And = 0
	/// Logical 'or'
	, Or
};

typedef std::function<bool()> Condition;
typedef std::function<void()> Trigger;
typedef std::function<QVariant()> Value;

typedef QMap<QString, QVariant> Variables;
typedef QMap<QString, Event *> Events;
typedef QMap<QString, QObject *> Objects;

}
}
}
