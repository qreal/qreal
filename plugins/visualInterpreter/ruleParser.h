#pragma once

#include "../blockDiagram/visualDebugSupport/interpreter/blockParser.h"

namespace qReal {

/// Rule action parser and interpreter
class RuleParser : public QObject, public utils::ExpressionsParser
{
	
public:
	RuleParser(LogicalModelAssistInterface const &logicalModelApi,
			GraphicalModelAssistInterface const &graphicalModelApi,
			ErrorReporterInterface* errorReporter);

	void parseRule(QString const &stream, QHash<Id, Id> *mMatch);
	void setRuleId(Id const &id);
	
private:
	void parseRuleCommand(QString const &stream, int &pos, QHash<Id, Id> *mMatch);
	void parseRunFunction(QString const &stream, int &pos, QHash<Id, Id> *mMatch);
	void parsePropertyChange(QString const &stream, int &pos, QHash<Id, Id> *mMatch);
	
	bool checkForLogicalConst(QString const &stream, int &pos);
	
	/// Parse from stream declaration of variables and calcule its values
	virtual void parseVarPart(QString const &stream, int &pos);
	
	/// Checking for the existence of variable declaration
	virtual void checkForVariable(QString const &nameOfVariable, int &index);
	
	QString parseElemName(QString const &stream, int &pos);
	QString parseElemProperty(QString const &stream, int &pos);
	
	void checkForClosingBracketAndColon(QString const &stream, int &pos);
	
	Id getElementByName(QString const &name, QHash<Id, Id> *mMatch);
	
	void setProperty(Id const &id, QString const &propertyName, QVariant const &value);
	QVariant getProperty(Id const &id, QString const &propertyName);
	
	LogicalModelAssistInterface const &mLogicalModelApi;
	GraphicalModelAssistInterface const &mGraphicalModelApi;
	
	Id mRuleId;
};

}
