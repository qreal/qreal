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

#include "triggersFactory.h"

#include "event.h"

using namespace twoDModel::constraints::details;

TriggersFactory::TriggersFactory(Events &events, Variables &variables, StatusReporter &status)
	: mEvents(events)
	, mVariables(variables)
	, mStatus(status)
{
}

Trigger TriggersFactory::doNothing() const
{
	return [](){};
}

Trigger TriggersFactory::fail(const QString &message) const
{
	return [this, message]() { emit mStatus.fail(message); };
}

Trigger TriggersFactory::success(bool deferred) const
{
	return [this, deferred]() { emit mStatus.success(deferred); };
}

Trigger TriggersFactory::combined(const QList<Trigger> &triggers) const
{
	return [triggers]() {
		for (const Trigger &trigger : triggers) {
			trigger();
		}
	};
}

Trigger TriggersFactory::setVariable(const QString &name, const Value &value) const
{
	return [this, name, value]() { mVariables[name] = value(); };
}

Trigger TriggersFactory::setObjectState(const Value &object, const QString &property, const Value &value) const
{
	return [this, object, property, value] {
		const QVariant variantObject = object();
		if (!variantObject.canConvert<QObject *>()) {
			reportError(QObject::tr("Invalid <setState> object type %1").arg(variantObject.typeName()));
		}

		QObject * const objectInstance = variantObject.value<QObject *>();
		const int index = objectInstance->metaObject()->indexOfProperty(qPrintable(property));
		if (index < 0) {
			reportError(QObject::tr("Object %1 has no property %2").arg(variantObject.typeName(), property));
			return;
		}

		objectInstance->setProperty(qPrintable(property), value());
	};
}

Trigger TriggersFactory::setUpEvent(const QString &id) const
{
	return [this, id]() {
		if (!mEvents.contains(id)) {
			reportError(QObject::tr("No such event: %1").arg(id));
			return;
		}

		mEvents[id]->setUp();
	};
}

Trigger TriggersFactory::dropEvent(const QString &id) const
{
	return [this, id]() {
		if (!mEvents.contains(id)) {
			reportError(QObject::tr("No such event: %1").arg(id));
			return;
		}

		mEvents[id]->drop();
	};
}

void TriggersFactory::reportError(const QString &message) const
{
	emit mStatus.checkerError(message);
}
