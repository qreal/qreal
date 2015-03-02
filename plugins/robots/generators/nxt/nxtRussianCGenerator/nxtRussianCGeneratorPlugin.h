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

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;
	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

	QList<qReal::ActionInfo> customActions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;

//	QList<qReal::ActionInfo> actions() override;
//	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	void regenerateExtraFiles(const QFileInfo &newFileInfo) override;
	QString defaultFilePath(const QString &projectName) const override;
	qReal::text::LanguageInfo language() const override;
	QString generatorName() const override;

private:
	/// Action that launches code generator
	QAction *mGenerateCodeAction;  // Doesn't have ownership; may be disposed by GUI.
};

}
}
