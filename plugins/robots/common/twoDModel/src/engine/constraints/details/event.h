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

#include "defines.h"

namespace twoDModel {
namespace constraints {
namespace details {

/// Represent a main entity of 2D model constraints language. Event is an entity that just waits for condition and
/// then fires trigger. It may be setted up or dropped (i.e. has "aliveness" state). In dropped state the trigger
/// will never be fired, even if condition is satisfied.
class Event : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param id The unique identifier of the event object.
	///    Events in program may be setted up or dropped using this identifier.
	/// @param condition Bool functor object that returns true if this event must be fired immediately.
	/// @param trigger Functor object that makes something that must be done on the event fire.
	/// @param dropsOnFire If true (default case) the event will be automaticly dropped when fired.
	///    Otherwise the event can fire multiple times without manual resetting.
	Event(const QString &id
		, const Condition &condition
		, const Trigger &trigger
		, bool dropsOnFire = true
		, bool isSettedInitially = false);

	/// Returns the unique identifier of this event that can be used for manual setting up or dropping this event.
	QString id() const;

	/// Returns true if the event is setted up (i.e. listens for condition to be satisfied and then fires)
	/// or false if it is dropped.
	bool isAlive() const;

	/// Returns true if the event is active from the start of the interpretation.
	bool isAliveInitially() const;

	/// Starts listening for the condition to be satisfied to fire the trigger then.
	void setUp();

	/// Stops listening for the condition to be satisfied; the event will never fire when it is dropped.
	void drop();

	/// Checks the given in constructor condition; if it returns true then fires the trigger and probably drops down
	/// (if dropsOnFire flag in constructor setted with true).
	/// The event can still be dropped in case when condition is not satisfied. That is made for example
	/// by "timer" condition with forceDrop flag setted to true.
	void check();

	/// Sets new condition to this event. This may be useful when condition instantiation requires the event instance
	/// (like in case of "timer" condition).
	void setCondition(const Condition &condition);

signals:
	/// Emitted when this event was setted up by someone even if event was already alive.
	void settedUp();

	/// Emitted when this event was dropped by someone even if event was already dropped.
	void dropped();

	/// Emitted when the condition was finally satisfied and the event was alive at that moment. This event can be only
	/// fired when check() is called.
	void fired();

private:
	const QString mId;
	bool mIsAlive;
	Condition mCondition;
	const Trigger mTrigger;
	bool mDropsOnFire;
	const bool mIsSettedInitially;
};

}
}
}
