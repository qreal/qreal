#pragma once

#include <nxtGeneratorBase/nxtGeneratorPluginBase.h>

namespace nxt {
namespace russianC {

/// Main plugin class for russian C code generator
class NxtRussianCGeneratorPlugin : public NxtGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "nxt.NxtRussianCGeneratorPlugin")

public:
	NxtRussianCGeneratorPlugin();

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
	/// Action that launches code generator
	QAction *mGenerateCodeAction;  // Doesn't have ownership; may be disposed by GUI.
};

}
}
