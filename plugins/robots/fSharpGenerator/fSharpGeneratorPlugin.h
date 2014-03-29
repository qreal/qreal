#pragma once

#include <robotsGeneratorPluginBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

/// Generation of QtScript program for TRIK on F#, uploading and execution of a program.

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
