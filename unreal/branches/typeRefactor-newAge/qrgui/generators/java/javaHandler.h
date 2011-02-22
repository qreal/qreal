#pragma once

#include <QtCore/QString>

#include "../../kernel/ids.h"
#include "../../../qrrepo/logicalRepoApi.h"

namespace qReal {

namespace generators {

class JavaHandler
{
public:
	explicit JavaHandler(qrRepo::LogicalRepoApi const &api);

	QString generateToJava(QString const &pathToDir);
	QString parseJavaLibraries(QString const &pathToDir);
private:
	bool checkTheModel();
	bool commentAndConstraintChecking(IdList const &idList,
			QString const &diagramType, QString const &nodeType);

	IdList getActivityChildren(Id const &idStartNode, Id const &untilNode);

	QString serializeObject(Id const &type);
	QString serializeChildren(Id const &type);
	QString serializeActivity(Id const &idStartNode, Id const &idUntilNode);

	QString getVisibility(Id const &type);
	QString getMultiplicity(Id const &type);
	QString getType(Id const &type);
	QString getDefaultValue(Id const &type);
	QString getImports(Id const &type);
	QString getConstraints(Id const &type);
	QString getConstraint(Id const &type);
	QString getComments(Id const &type);
	QString getComment(Id const &type);
	QString getOperationFactors(Id const &type);
	QString hasModifier(Id const &type, QString const &modifier);
	QString getSuperclass(Id const &type);
	QString getInterfaces(Id const &type);
	QString getMethodCode(Id const &type);
	QString getFlowGuard(Id const &type);
	QString serializeMultiplicity(Id const &type, QString const &multiplicity) const;

	bool isTypeSuitable(QString const &type) const;
	bool isVisibilitySuitable(QString const &type) const;

	QString objectType(Id const &type);
	QString tryCatch(Id const &type);
	QString ifStatement(Id const &type);
	QString whileDoLoop(Id const &type);

	Id findMergeNode(Id const &idDecisionNode);
	Id findNonBodyLink(Id const &idDecisionNode);
	IdList findIntermediateNodes(Id const &type, Id const &untilNode, bool const closesFunction);
	IdList deleteCommentLinks(IdList &idList);
	IdList deleteConstraintEdges(IdList &idList);

	void addError(QString const &errorText);

	qrRepo::LogicalRepoApi const &mApi;
	QString mErrorText;
	QString pathToDir;

	int mIndent;
	QString indent();

	//Parsing Java Libraries
	QStringList getAllFilesInDirectory(QString dir_name);
};

}
}
