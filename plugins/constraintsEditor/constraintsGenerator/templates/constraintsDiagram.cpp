#include "constraints@@diagramName@@.h"
#include <QtCore/QVariant>

using namespace constraints;

Constraints@@diagramName@@::Constraints@@diagramName@@()
{
}

Constraints@@diagramName@@::~Constraints@@diagramName@@()
{
}

@@countOptionalCheckStatusesForElemets@@

@@countMainCheckStatusesForElemets@@
QList<qReal::CheckStatus> Constraints@@diagramName@@::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	mEditorManager = &editorManager;
@@prefixForReturnCheckStatusesOfElementsInCheck@@
@@returnCheckStatusesOfElementsInCheck@@
	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString Constraints@@diagramName@@::languageName() const
{
	return "@@languageName@@";
}

QList<QString> Constraints@@diagramName@@::elementsNames() const
{
	QList<QString> elementsList;
@@addElementsInElementsNames@@
	return elementsList;
}
