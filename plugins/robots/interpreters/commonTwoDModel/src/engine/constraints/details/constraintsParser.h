#pragma once

#include "defines.h"

class QDomElement;

namespace twoDModel {
namespace constraints {
namespace details {

class ConstraintsParser
{
public:
	ConstraintsParser(Events &events, Variables &variables, Objects const &objects);

	bool parse(const QString &constrtaintsXml);
	QString errorString() const;

	int timeLimit() const;

private:
	void parseConstraints(const QDomElement &constraints);

	QString mError;
	Events &mEvents;
	Variables &mVariables;
	Objects const &mObjects;
};

}
}
}
