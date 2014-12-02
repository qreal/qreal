#pragma once

#include <trikGeneratorBase/trikGeneratorPluginBase.h>

namespace trik {

class TrikRuntimeUploaderPlugin : public TrikGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "trik.TrikRuntimeUploaderPlugin")

public:
	TrikRuntimeUploaderPlugin();

	QList<qReal::ActionInfo> actions() override;

private slots:
	void uploadRuntime();

private:
	generatorBase::MasterGeneratorBase *masterGenerator() override;

	QAction *mAction;  // Doesn't have ownership; may be disposed by GUI.
};

}
