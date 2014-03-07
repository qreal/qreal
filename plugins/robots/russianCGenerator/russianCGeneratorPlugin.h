#pragma once

#include <robotsGeneratorPluginBase.h>

namespace qReal {
namespace robots {
namespace generators {

/// Main plugin class for russian C code generator
class RussianCGeneratorPlugin : public RobotsGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "qReal.robots.generator.RussianCGeneratorPlugin")

public:
	RussianCGeneratorPlugin();
	virtual ~RussianCGeneratorPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

protected:
	virtual MasterGeneratorBase *masterGenerator();
	virtual void regenerateExtraFiles(QFileInfo const &newFileInfo);
	virtual QString defaultFilePath(QString const &projectName) const;
	virtual QString extension() const;
	virtual QString extDescrition() const;
	virtual QString generatorName() const;

private:
	/// Disable/enable tab in QList<ActionInfo> info
	void changeActiveTab(QList<qReal::ActionInfo> const &info, bool trigger);

	/// Action that launches code generator
	QAction mGenerateCodeAction;

	QTranslator mAppTranslator;
};

}
}
}
