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

#include <qrgui/mainWindow/errorReporter.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>

namespace qReal {

/// Generates text code file for reaction on the rule application specified in "procedure" attribute of "Rule" element
class TextCodeGenerator : public QObject
{
	Q_OBJECT

public:
	/// Delimiter that will be inserted instead of '.' in each "elemName.propertyName" occurencce
	static QString const delimeter;

	TextCodeGenerator(LogicalModelAssistInterface &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface);

	/// Matched rule id
	void setRule(Id const &rule);

	/// Matched rule match
	void setMatch(QHash<Id, Id> const &match);

	/// Generate and return reaction script or application condition script
	virtual QString generateScript(bool const isApplicationCondition);

	/// Returns element id by it's name (from single rule)
	Id idByName(QString const &name) const;

protected:
	/// Checks if rule have element with given name
	bool hasElementName(QString const &name) const;

	bool hasProperty(Id const &element, QString const &propertyName) const;
	virtual QString property(Id const &element, QString const &propertyName) const;
	QVariant propertyVariant(Id const &element, QString const &propertyName) const;

	/// Checks if property has string type
	bool isStringProperty(Id const &element, QString const &propertyName) const;

	/// Collects all pairs of elemName.propertyName (property usage) and elemName.propertyName() (method invocation)
	/// in specified code piece
	void collectPropertiesUsageAndMethodsInvocation(QString const &code);

	/// Replaces all properties usage with appropriate variable name with delimeter
	QString replacePropertiesUsage(QString const &code) const;

	/// Replaces all methods invocation with appropriate function name with delimeter and add definition of this
	/// function on top of the code
	QString replaceMethodsInvocation(QString const &code) const;

	/// Substitute all occurrences of elemName@propertyName with content of this property
	QString substituteElementProperties(QString const &code) const;

	/// Add to code correct initialization of new variables and create proper output for model update
	virtual QString createProperInitAndOutput(QString const &code, bool const isApplicationCondition) const = 0;

	/// Create function definition from element property
	virtual QString createBehaviourFunction(QString const &elementName, QString const &propertyName) const = 0;

	/// Prepare property value for insertion in function definition (replace this. usages, add global variable, etc)
	virtual QString properElementProperty(QString const &elementName, QString const &propertyName) const = 0;

	QString parseIdentifier(QString const &stream, int pos, bool leftToRight) const;
	bool isCorrectIdentifierSymbol(QChar const c) const;

	/// Escapes string (',",\,\t,\r,\n)
	QString escape(QString const &string) const;

	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	LogicalModelAssistInterface &mLogicalModelApi;
	GraphicalModelAssistInterface &mGraphicalModelApi;

	Id mRule;
	IdList mRuleElements;
	QHash<Id, Id> mMatch;

	QHash<QString, QSet<QString>* > mPropertiesUsage;
	QHash<QString, QSet<QString>* > mMethodsInvocation;
};

}
