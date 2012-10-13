#pragma once

#include <QtCore/QDir>

#include "../../qrgui/mainwindow/errorReporter.h"
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace qReal {

/// Generates python script file for reaction on the rule application specified in "procedure" attribute of "Rule" element
class PythonGenerator : public QObject
{
	Q_OBJECT

public:
	/// Delimiter that will be inserted instead of '.' in each "elemName.propertyName" occurencce
	static QString const delimeter;

	/// Constructor
	/// @param scriptPath Where save reaction on python
	PythonGenerator(LogicalModelAssistInterface &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, QString const &scriptPath = QDir().currentPath() + "/reaction.py");

	/// Matched rule id
	void setRule(Id const &rule);

	/// Matched rule match
	void setMatch(QHash<Id, Id> const &match);

	/// Sets path where will be saved reaction on python
	void setScriptPath(QString const &path);

	/// Generate and save reaction script on python
	void generateScript();

	/// Returns element id by it's name (from single rule)
	Id idByName(QString const &name) const;

private:
	/// Checks if rule have element with given name
	bool hasElementName(QString const &name) const;

	bool hasProperty(Id const &element, QString const &propertyName) const;
	QString property(Id const &element, QString const &propertyName) const;
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
	QString replaceMethodsInvocation(QString const &reaction) const;

	/// Substitute all occurrences of elemName@propertyName with content of this property
	QString substituteElementProperties(QString const &reaction) const;

	/// Add to code correct initialization of new variables and create proper output for model update
	QString createProperInitAndOutput(QString const &reaction) const;

	/// Create function definition from element property
	QString createBehaviourFunction(QString const &elementName, QString const &propertyName) const;

	/// Prepare property value for insertion in function definition (replace this. usages, add global variable, etc)
	QString properElementProperty(QString const &elementName, QString const &propertyName) const;

	QString parseIdentifier(QString const &stream, int pos, bool leftToRight) const;
	bool isCorrectIdentifierSymbol(QChar const c) const;

	/// Writes script in file
	void createScriptFile(QString const &script) const;

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

	QString mScriptPath;
};

}
