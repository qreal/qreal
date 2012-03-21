#pragma once

#include "../../../qrgui/mainwindow/errorReporter.h"
#include "../../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace qReal {

/// Graph transformation unit performs different transformation of graphic model
/// according to contained rules which can contain creation, removal, replacement
/// (etc) of elements.
class GraphTransformationUnit : public QObject
{
	Q_OBJECT

public:
	GraphTransformationUnit(LogicalModelAssistInterface const &logicalModelApi
			, GraphicalModelAssistInterface const &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface);
	~GraphTransformationUnit();
	
	/// Load semantics model from current open diagram
	void loadSemantics();
	
	/// Make one step according to semantics (find match, delete, create and replace elements)
	void interpret();
	
private:
	void findMatch();
	bool checkOneRuleForMatch(Id const &rule);
	
	/// Perform all transformations
	void makeStep();
	
	/// Fill rules information with this
	void putIdIntoMap(QMap<QString ,QList<Id>* > *map, QString const &ruleName,
			Id const &id);
	
	/// Obtain an element id with the corresponding control flow mark
	Id getNodeIdWithControlMark(Id const &controlMarkId) const;
	
	QVariant getProperty(Id const &id, QString const &propertyName);
	
	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	LogicalModelAssistInterface const &mLogicalModelApi;
	GraphicalModelAssistInterface const &mGraphicalModelApi;
	
	/// Metamodel name which loaded semantics is for
	QString mMetamodelName;
	
	/// All rules in map with key - rule name and value - rule id on diagram
	QMap<QString, Id> *mRules;
	
	/// All maps below has the format: key - rule name, value - list of ids
	QMap<QString, QList<Id>* > *mDeletedElements;
	QMap<QString, QList<Id>* > *mReplacedElements;
	QMap<QString, QList<Id>* > *mCreatedElements;
	QMap<QString, QList<Id>* > *mElementsWithNewControlMark;
	QMap<QString, QList<Id>* > *mElementsWithControlMark;
	
	/// Match map: key - id in rule diagram, value - id in diagram which will be transformed
	QMap<Id, Id> *mMatch;
	
	QString mMatchedRuleName;
};

}
