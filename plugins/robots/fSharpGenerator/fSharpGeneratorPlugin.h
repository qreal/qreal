#pragma once

#include <robotsGeneratorPluginBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

/// Generation of QtScript program for FSharp, uploading and execution of a program.
/// Uses setting "tcpServer" from RobotsInterpreter.
class FSharpGeneratorPlugin : public RobotsGeneratorPluginBase
{
	Q_OBJECT
    Q_PLUGIN_METADATA(IID "qReal.robots.FSharpGenerator.FSharpGeneratorPlugin")

public:
    FSharpGeneratorPlugin();

    virtual ~FSharpGeneratorPlugin();

	virtual QList<qReal::ActionInfo> actions();

protected:
	virtual MasterGeneratorBase *masterGenerator();
	virtual void regenerateExtraFiles(QFileInfo const &newFileInfo);
	virtual QString defaultFilePath(QString const &projectName) const;
	virtual QString extension() const;
	virtual QString extDescrition() const;
	virtual QString generatorName() const;

private:
	/// Action that launches code generator
	QAction mGenerateCodeAction;

	QTranslator mAppTranslator;
};

}
}
}
}
