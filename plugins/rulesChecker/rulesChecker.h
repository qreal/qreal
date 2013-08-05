﻿#pragma once

#include "../../qrgui/mainwindow/projectManager/projectManagementInterface.h"
#include "../../qrgui/toolPluginInterface/toolPluginInterface.h"

namespace qReal{
namespace rulesChecker{

//! @class RulesChecker watches current diagram for errors and makes report
class RulesChecker : public QObject
{
	Q_OBJECT

public:
	RulesChecker(qrRepo::GraphicalRepoApi const &graphicalRepoApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface);
	// default destructor is ok


public slots:
	void checkAllDiagrams();
	void checkCurrentDiagram();
	//! get an XML file with all repo contents (used as a hack for integration with REAL-IT.NET)
	void exportToXml();

private:
	 enum ErrorsType {
		linkToStartNode
		, linkFromFinalNode
		, noStartNode
		, noEndNode
		, incorrectLink
	};

	//! starts DFS for all connected components
	void checkDiagram();

	//! DFS, checks rule that all paths have start with StartEvent and finish in EndEvent
	//! removes elements from mDiagramModels list while making detour
	//! @arg usedNodes prevents detour to go twice in same node
	//! @returns bool true if have reached the final node
	bool makeDetour(Id const &currentNode, IdList &usedNodes);

	//! controls existsing nodes at the ends of link
	void checkLinksRule(Id const &key);
	//! controls that start node hasn't got incoming links, so as end-node outcoming
	void checkFinalNodeRule(Id const &node);

	//! clears errorlog
	void prepareOutput();
	//! makes report and highlights of badNode
	void postError(ErrorsType const error, Id const &badNode);

	bool isLink(Id const &node) const;
	bool isContainer(Id const &node) const;
	bool isStartNode(Id const &node) const;
	bool isEndNode(Id const &node) const;

	//! @returns IdList list all graphical elements of diagram
	IdList elementsOfDiagram(Id const &diagram) const;

	//! removes containers from elements list, checks link-rule and final-nodes-rule
	void checkDiagramElements();
	//! @return start-nodes from diagram list
	IdList collectStartNodes() const;

	//! @return Id node with minimal incoming links count
	Id findFirstNode() const;

	IdList incomingSequenceFlow(Id const &id) const;
	IdList outgoingSequenceFlow(Id const &id) const;

	qrRepo::GraphicalRepoApi const *mGRepoApi;
	qReal::gui::MainWindowInterpretersInterface *mWindowInterface;

	QStringList mLinkTypes;
	QStringList mContainerTypes;

	//! contains all elements from current diagram
	IdList mDiagramElements;
	//! main flag
	bool mNoErrorsOccured;
};

}
}

