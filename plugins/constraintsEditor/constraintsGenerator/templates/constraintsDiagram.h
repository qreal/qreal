#pragma once
#include "../../../qrgui/constraintsPluginInterface/constraintsPluginInterface.h"

namespace constraints {

class Constraints@@diagramName@@
{
public:
	Constraints@@diagramName@@();
	virtual ~Constraints@@diagramName@@();

@@optionalChecksForElemets@@
@@mainChecksForElemets@@
	qReal::CheckStatus check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QString languageName() const;
	QList<QString> elementsNames() const;
};

}

