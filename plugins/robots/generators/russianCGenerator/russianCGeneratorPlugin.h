#pragma once

#include <generatorBase/robotsGeneratorPluginBase.h>

namespace russianC {

/// Main plugin class for russian C code generator
class RussianCGeneratorPlugin : public generatorBase::RobotsGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "russianC.RussianCGeneratorPlugin")

public:
	RussianCGeneratorPlugin();
	virtual ~RussianCGeneratorPlugin();

	QString kitId() const override;

	virtual QList<qReal::ActionInfo> actions();

protected:
	virtual generatorBase::MasterGeneratorBase *masterGenerator();
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
