#pragma once

#include "../../../qrgui/mainwindow/projectManager/projectManagementInterface.h"
#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"

namespace qReal{
namespace rulesChecker{

//! @class RulesChecker watches current diagram for errors and makes report
class RulesChecker : public QObject
{
	Q_OBJECT

public slots:
	void checkAllDiagrams();
	void checkCurrentDiagram();

public:
	RulesChecker(qrRepo::GraphicalRepoApi const &graphicalRepoApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface);
	// std destructor ok

private:
	 enum ErrorsType {
		LinkToStartNode
		, LinkFromFinalNode
		, NoStartNode
		, NoEndNode
		, IncorrectLink
	};

	//! starts DFS for all connected component
	void researchDiagram();

	//! DFS, checks rule that all paths have start with StartEvent and finish in EndEvent
	//! removes elements from mDiagramModels list while making detour
	//! @arg usedNodes prevents detour to go twice in same node
	//! @returns bool true if have reached the final node
	bool makeDetour(Id const currentNode, IdList &usedNodes);

	//! controls existsing nodes at the ends of link
	void checkLinksRule(Id const &key);
	//! controls that start node hasn't got incoming links, so as end-node outcoming
	void checkFinalNodeRule(Id const &key);

	//! clears errorlog
	void prepareOutput();
	//! makes report and highlights of badNode
	void postError(ErrorsType const error, Id badNode);

	bool isLink(Id const &model) const;
	bool isStartNode(Id const &model) const;
	bool isEndNode(Id const &model) const;

	//! @returns IdList list all graphical elements of diagram
	IdList elementsOfDiagram(Id const &diagram) const;

	//! removes containers from list, checks link-rule and final-nodes-rule
	//! @return IdList of head-nodes
	IdList checkDiagramModelsList(IdList &list);

	//! @return Id node with minimal incoming links count
	Id findFirstNode(Id const &key) const;

	//! @arg result will have all nodes that have path to node key
	void getPreviousNodes(Id const &key, IdList &result) const;

	qrRepo::GraphicalRepoApi const *mGRepoApi;
	qReal::gui::MainWindowInterpretersInterface *mWindowInterface;

	//! TODO: find better place for that
	QStringList linkTypes;
	QStringList containerTypes;

	//! consists of models from current diagram
	IdList mDiagramModels;
	//! main flag
	bool hasNoErrors;
};

}
}

