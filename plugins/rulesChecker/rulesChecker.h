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
	void prepareOutput();
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
	//! DFS, removes elements from metamodel list while making detour
	//! @arg goForwards guides DFS so nodes directions are, against else
	//! @returns bool reached the final node
	bool makeDetour(Id const currentNode, IdList &used, bool const goForwards = true);

	bool isValidFinalNode(Id const &node);

	void postError(ErrorsType const error, Id badNode);
	bool isLink(Id const &model);
	//!
	IdList childrenOfDiagram(Id const &diagram);

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

