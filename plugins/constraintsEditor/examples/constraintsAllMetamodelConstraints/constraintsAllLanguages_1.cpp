#include "constraintsAllLanguages_1.h"
#include <QtCore/QVariant>

using namespace constraints;

ConstraintsAllLanguages_1::ConstraintsAllLanguages_1()
{
}

ConstraintsAllLanguages_1::~ConstraintsAllLanguages_1()
{
}

qReal::CheckStatus ConstraintsAllLanguages_1::checkAllNodes_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;
			int tempCountIncomingLinks_1 = 0;
		qReal::IdList newIncomingLinksNamesList_1 = logicalApi.incomingLinks(element);
		bool mainIncomingLinksRes_2 = true;
		foreach (qReal::Id const &edgeIncomingLinks_1, newIncomingLinksNamesList_1) {
				bool incomingLinksRes_2 = true;
			mainIncomingLinksRes_2 = mainIncomingLinksRes_2 && incomingLinksRes_2;
			tempCountIncomingLinks_1++;
		}
		bool countIncomingLinksRes_1 = (tempCountIncomingLinks_1 < 2);
		int tempCountOutgoingLinks_1 = 0;
		qReal::IdList newOutgoingLinksNamesList_1 = logicalApi.outgoingLinks(element);
		bool mainOutgoingLinksRes_2 = true;
		foreach (qReal::Id const &edgeOutgoingLinks_1, newOutgoingLinksNamesList_1) {
				bool outgoingLinksRes_2 = true;
			mainOutgoingLinksRes_2 = mainOutgoingLinksRes_2 && outgoingLinksRes_2;
			tempCountOutgoingLinks_1++;
		}
		bool countOutgoingLinksRes_1 = (tempCountOutgoingLinks_1 > 2);
		int tempCountIncomingLinks_2 = 0;
		qReal::IdList newIncomingLinksNamesList_2 = logicalApi.incomingLinks(element);
		bool mainIncomingLinksRes_3 = true;
		foreach (qReal::Id const &edgeIncomingLinks_2, newIncomingLinksNamesList_2) {
				bool incomingLinksRes_3 = true;
			mainIncomingLinksRes_3 = mainIncomingLinksRes_3 && incomingLinksRes_3;
			tempCountIncomingLinks_2++;
		}
		bool countIncomingLinksRes_2 = (tempCountIncomingLinks_2 < 2);
			bool allNodesRes_1 = ( (mainIncomingLinksRes_2 && countIncomingLinksRes_1) || ( (mainOutgoingLinksRes_2 && countOutgoingLinksRes_1) || (mainIncomingLinksRes_3 && countIncomingLinksRes_2) ) );
	res = allNodesRes_1;

	return qReal::CheckStatus(res, "fail_text", qReal::CheckStatus::warning);
}


QList<qReal::CheckStatus> ConstraintsAllLanguages_1::checkAllNodes(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkAllNodes_1(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QList<qReal::CheckStatus> ConstraintsAllLanguages_1::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	mEditorManager = &editorManager;
	qReal::EditorManagerInterface::MetaType isNodeOrEdge = mEditorManager->metaTypeOfElement(element);

	if(isNodeOrEdge == qReal::EditorManagerInterface::node) {
		checkings.append(checkAllNodes(element, logicalApi));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsAllLanguages_1::languageName() const
{
	return "AllLanguages";
}

QList<QString> ConstraintsAllLanguages_1::elementsNames() const
{
	QList<QString> elementsList;
	elementsList.append("AllNodes");

	return elementsList;
}
