#pragma once

#include "details/defines.h"

namespace twoDModel {
namespace constraints {

class ConstraintsChecker : public QObject
{
	Q_OBJECT

public:
	ConstraintsChecker();
	~ConstraintsChecker();

	bool parseConstraints(const QString &constraintsXml);

private:
	details::Events mEvents;
	details::Variables mVariables;
	details::Objects mObjects;
};

}
}
