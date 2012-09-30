#pragma once

#include "../../qrgui/mainwindow/errorReporter.h"
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

#include <QtCore/QDir>

namespace qReal {

class PythonGenerator : public QObject
{
	Q_OBJECT

public:
	static QString const delimeter;

	PythonGenerator(LogicalModelAssistInterface &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, QString const &scriptPath = QDir().currentPath() + "/reaction.py");

	void setRule(Id const &rule);
	void setMatch(QHash<Id, Id> const &match);
	void setScriptPath(QString const &path);

	void generateScript();

private:
	Id idByName(QString const &name) const;
	bool hasElementName(QString const &name) const;
	bool hasProperty(Id const &element, QString const &propertyName) const;
	QString property(Id const &element, QString const &propertyName) const;
	QVariant propertyVariant(Id const &element, QString const &propertyName) const;
	bool isStringProperty(Id const &element, QString const &propertyName) const;

	void collectPropertiesUsageAndMethodsInvocation(QString const &code);
	QString replacePropertiesUsage(QString const &code) const;
	QString replaceMethodsInvocation(QString const &reaction) const;
	QString substituteElementProperties(QString const &reaction) const;
	QString createProperInitAndOutput(QString const &reaction) const;

	QString createBehaviourFunction(QString const &elementName, QString const &propertyName) const;
	QString properElementProperty(QString const &elementName, QString const &propertyName) const;
	QString parseIdentifier(QString const &stream, int pos, bool leftToRight) const;

	bool isCorrectIdentifierSymbol(QChar const c) const;

	void createScriptFile(QString const &script) const;

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
