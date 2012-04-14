#pragma once

#include "../blockDiagram/visualDebugSupport/interpreter/blockParser.h"

namespace qReal {

/// Textual part of rules parser and interpreter (property changes, calculations)
class RuleParser : public QObject, public utils::ExpressionsParser
{
	
public:
	RuleParser(LogicalModelAssistInterface const &logicalModelApi,
			GraphicalModelAssistInterface const &graphicalModelApi,
			ErrorReporterInterface* errorReporter);

	/// Parse and interpret textual part of rule and perform changes
	/// on model elements by using match hash map
	void parseRule(QString const &stream, QHash<Id, Id> *mMatch);
	
	/// Set matched rule id for showing if its textual part has errors
	void setRuleId(Id const &id);
	
private:
	/// Parse from stream declaration of variables and calcule its values
	virtual void parseVarPart(QString const &stream, int &pos);
	
	/// Parse and interpret one rule command (which are separated by colon)
	void parseRuleCommand(QString const &stream, int &pos, QHash<Id, Id> *mMatch);
	
	/// Parse and interpret command which has run(name.property) function
	/// (to interpret some commands inside given element)
	void parseRunFunction(QString const &stream, int &pos, QHash<Id, Id> *mMatch);
	
	/// Parse and interpret command which contains property change of some element
	void parsePropertyChange(QString const &stream, int &pos, QHash<Id, Id> *mMatch);
	
	/// Parse element name which will be changed in this command
	QString parseElemName(QString const &stream, int &pos);
	
	/// Parse element property to change
	QString parseElemProperty(QString const &stream, int &pos);
	
	bool checkForLogicalConst(QString const &stream, int &pos);
	void checkForClosingBracketAndColon(QString const &stream, int &pos);
	
	/// Checking for the existence of variable declaration
	virtual void checkForVariable(QString const &nameOfVariable, int &index);
	
	/// Gets id of element in model which corresponds to element
	/// in rule which has given name
	Id getElementByName(QString const &name, QHash<Id, Id> *mMatch);
	
	/// Functions for working with properties of elements on model
	void setProperty(Id const &id, QString const &propertyName, QVariant const &value);
	QVariant getProperty(Id const &id, QString const &propertyName);
	
	LogicalModelAssistInterface const &mLogicalModelApi;
	GraphicalModelAssistInterface const &mGraphicalModelApi;
	
	/// Matched rule id for showing if its textual part has errors
	Id mRuleId;
};

}
