#pragma once

#include "defines.h"

namespace twoDModel {
namespace constraints {
namespace details {

class TriggersFactory
{
public:
	TriggersFactory(Events &events, Variables &variables);

	Trigger doNothing() const;
	Trigger combined(const QList<Trigger> &triggers) const;

	Trigger fail(const QString &message) const;
	Trigger success() const;

	Trigger setVariable(const QString &name, const QString &value) const;
	Trigger addToVariable(const QString &name, const QString &value) const;

	Trigger setUpEvent(const QString &id) const;
	Trigger dropEvent(const QString &id) const;

private:
	Events &mEvents;
	Variables &mVariables;
};

}
}
}
