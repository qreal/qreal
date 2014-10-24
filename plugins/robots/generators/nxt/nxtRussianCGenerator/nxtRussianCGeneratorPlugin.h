#pragma once

#include <generatorBase/robotsGeneratorPluginBase.h>

namespace russianC {

/// Main plugin class for russian C code generator
class NxtRussianCGeneratorPlugin : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "russianC.RussianCGeneratorPlugin")

public:
	NxtRussianCGeneratorPlugin();
	~NxtRussianCGeneratorPlugin() override;

	QString kitId() const override;

	QList<qReal::ActionInfo> actions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	void regenerateExtraFiles(QFileInfo const &newFileInfo) override;
	QString defaultFilePath(QString const &projectName) const override;
	QString extension() const override;
	QString extensionDescription() const override;
	QString generatorName() const override;

private:
	/// Disable/enable tab in QList<ActionInfo> info
	void changeActiveTab(QList<qReal::ActionInfo> const &info, bool trigger);

	/// Action that launches code generator
	QAction mGenerateCodeAction;
};

}
