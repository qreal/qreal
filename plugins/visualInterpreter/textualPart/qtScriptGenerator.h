#pragma once

#include "../../qrgui/mainwindow/errorReporter.h"
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

#include "textCodeGenerator.h"

namespace qReal {

/// Generates QtScript script file for reaction on the rule application specified in "procedure" attribute of "Rule" element
class QtScriptGenerator : public TextCodeGenerator
{
	Q_OBJECT

public:
	QtScriptGenerator(LogicalModelAssistInterface &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface);

protected:
	/// Add to code correct initialization of new variables and create proper output for model update
	QString createProperInitAndOutput(QString const &code, bool const isApplicationCondition) const;

	/// Create function definition from element property
	QString createBehaviourFunction(QString const &elementName, QString const &propertyName) const;

	/// Prepare property value for insertion in function definition (replace this. usages, add global variable, etc)
	QString properElementProperty(QString const &elementName, QString const &propertyName) const;
};

}
