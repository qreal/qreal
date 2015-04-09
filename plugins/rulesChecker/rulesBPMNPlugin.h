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

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>

#include "rulesChecker.h"
#include "bpmnCustomizer.h"

namespace qReal{
namespace rulesChecker{

//! @class RulesPlugin allows to check diagram for BPMN rules
class RulesPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.rulesChecker.RulesPlugin")
public:
	RulesPlugin();
	~RulesPlugin();

	void init(PluginConfigurator const &configurator);

	qReal::Customizer *customizationInterface();

	QList<ActionInfo> actions();
private:
	//! init start key
	void initAction();

	RulesChecker *mChecker;
	BpmnCustomizer mCustomizer;

	QList<qReal::ActionInfo> mActionInfos;

	//! starts checking
	QAction *mRunAllDiagram;
	QAction *mRunCurrentDiagram;
	QAction *mExportToXml;
};

}
}


