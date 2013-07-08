#pragma once

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"

#include "rulesChecker.h"

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

	void init(const PluginConfigurator &configurator);

	QList<ActionInfo> actions();
private:
	//! init start key
	void initAction();

	RulesChecker *mChecker;

	QList<qReal::ActionInfo> mActionInfos;

	//! starts checking
	QAction *mRunAllDiagram;
	QAction *mRunCurrentDiagram;
};

}
}


