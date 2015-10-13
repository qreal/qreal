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

#include "textCodeGenerator.h"

namespace qReal {

/// Generates QtScript script file for reaction on the rule application specified in "procedure" attribute of
/// "Rule" element.
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
