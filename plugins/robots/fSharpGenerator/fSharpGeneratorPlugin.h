#pragma once

#include <robotsGeneratorPluginBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

/// Plugin for generation code on F#
///
class FSharpGeneratorPlugin : public RobotsGeneratorPluginBase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qReal.robots.FSharpGenerator.FSharpGeneratorPlugin")

public:
    FSharpGeneratorPlugin();

    ~FSharpGeneratorPlugin() override;

    QList<qReal::ActionInfo> actions() override;

protected:
    virtual MasterGeneratorBase *masterGenerator() override;
    virtual void regenerateExtraFiles(QFileInfo const &newFileInfo) override;
    virtual QString defaultFilePath(QString const &projectName) const override;
    virtual QString extension() const override;
    virtual QString extDescrition() const override;
    virtual QString generatorName() const override;

    /// Translator object for this plugin
    QTranslator mAppTranslator;

private:
    /// Action that launches code generator
    QAction mGenerateCodeAction;

    /// Action that generates and uploads program on a robot
    QAction mUploadProgramAction;

    /// Action that generates and uploads program on a robot
    QAction mRunProgramAction;

    /// Action that stops script execution and turns off motors.
    QAction mStopRobotAction;

    QTranslator mAppTranslator;
};

}
}
}
}
