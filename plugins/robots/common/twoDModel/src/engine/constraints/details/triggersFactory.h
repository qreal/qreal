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

/// Produces functor objects that implement behaviour for tags in "trigger" group.
/// Produced functors implement logic that user requires to be invoked when parent event is fired.
class TriggersFactory
{
public:
	TriggersFactory(Events &events, Variables &variables, StatusReporter &status);

	/// Produces new trigger that does nothing.
	Trigger doNothing() const;

	/// Produces new trigger that sequentially invokes the given triggers.
	Trigger combined(const QList<Trigger> &triggers) const;

	/// Produces new trigger that sends back to the environment fail signal and passes the given message as the reason.
	Trigger fail(const QString &message) const;

	/// Produces new trigger that sends back to the environment sucess signal (i.e. that the program worked correctly
	/// and all conditions were satisfied).
	Trigger success() const;

	/// Produces new trigger that assigns to the given variable the given value. Declares new variable
	/// if it does not exist.
	Trigger setVariable(const QString &name, const QVariant &value) const;

	/// Produces new trigger that adds the given value to the given variable.
	/// Type of the passed value may differ from the old one. Then best addition overload will be selected.
	/// That means if values can`t be added like numbers then the result will be a concatenation of two strings.
	Trigger addToVariable(const QString &name, const QVariant &value) const;

	/// Enables the event with the given id i.e. it will listen for its condition to satisfy and fire then.
	Trigger setUpEvent(const QString &id) const;

	/// Disables the event with the given id i.e. it will stop waiting for the moment to fire.
	Trigger dropEvent(const QString &id) const;

private:
	void reportError(const QString &message);

	Events &mEvents;
	Variables &mVariables;
	StatusReporter &mStatus;
};

}
}
}
