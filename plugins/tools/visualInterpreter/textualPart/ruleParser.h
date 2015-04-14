/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <qrutils/expressionsParser/expressionsParser.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>

namespace qReal {

/// Textual part of rules parser and interpreter (property changes, calculations)
class RuleParser : public QObject, public utils::ExpressionsParser
{

public:
	RuleParser(LogicalModelAssistInterface &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, ErrorReporterInterface* errorReporter);

	/// Parse and interpret textual part of rule and perform changes
	/// on model elements by using match hash map
	bool parseRule(QString const &stream, QHash<Id, Id> const &mMatch);

	/// Parse and interpret application condition of the rule
	bool parseApplicationCondition(QString const &stream, QHash<Id, Id> const &mMatch);

	/// Interpret code, represented as string (can't contain attributes of model elements)
	void parseStringCode(QString const &stream);

	/// Set matched rule id for showing if its textual part has errors
	void setRuleId(Id const &id);

private:
	/// Parse declaration of variables from stream and calcule its' values
	virtual void parseVarPart(QString const &stream, int &pos);

	/// Parse and interpret one rule command (which are separated by colon)
	void parseRuleCommand(QString const &stream, int &pos, QHash<Id, Id> const &mMatch);

	/// Parse and interpret command which has run(name.property) function
	/// (to interpret some commands inside given element)
	void parseRunFunction(QString const &stream, int &pos, QHash<Id, Id> const &mMatch);

	/// Parse and interpret command which contains change of some element's property
	void parsePropertyChange(QString const &stream, int &pos, QHash<Id, Id> const &mMatch);

	/// Parse element name which will be changed in this command
	QString parseElemName(QString const &stream, int &pos);

	/// Parse element property to change
	QString parseElemProperty(QString const &stream, int &pos);

	bool checkForLogicalConst(QString const &stream, int &pos);
	void checkForClosingBracketAndColon(QString const &stream, int &pos);

	/// Check for variable declaration
	virtual void checkForVariable(QString const &nameOfVariable, int &index);

	/// Get id of element in model which corresponds to element in rule which has given name
	Id elementByName(QString const &name, QHash<Id, Id> const &mMatch);

	/// Functions for working with properties of elements on model
	void setProperty(Id const &id, QString const &propertyName, QVariant const &value);
	QVariant property(Id const &id, QString const &propertyName);

	LogicalModelAssistInterface &mLogicalModelApi;
	GraphicalModelAssistInterface &mGraphicalModelApi;

	/// Matched rule id for showing if its textual part has errors
	Id mRuleId;
};

}
