#include "fSharpGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>

#include "fSharpMasterGenerator.h"

using namespace qReal;
using namespace qReal::robots::generators;
using namespace fSharp;

FSharpGeneratorPlugin::FSharpGeneratorPlugin()
    : mGenerateCodeAction(NULL)
    , mUploadProgramAction(NULL)
    , mRunProgramAction(NULL)
    , mStopRobotAction(NULL)
{
    mAppTranslator.load(":/sampleGenerator_" + QLocale::system().name());
    QApplication::installTranslator(&mAppTranslator);
}

FSharpGeneratorPlugin::~FSharpGeneratorPlugin()
{
}

QList<ActionInfo> FSharpGeneratorPlugin::actions()
{
    QAction * const generateCodeAction = new QAction(tr("Generate F# code"), this);
    ActionInfo generateCodeActionInfo(generateCodeAction, "generators", "tools");
    connect(generateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

    return { generateCodeActionInfo };
}

MasterGeneratorBase *FSharpGeneratorPlugin::masterGenerator()
{
    return new FSharpMasterGenerator(*mRepo
                                     , *mMainWindowInterface->errorReporter()
                                     , mMainWindowInterface->activeDiagram());
}

void FSharpGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
    Q_UNUSED(newFileInfo);
}

QString FSharpGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
    return QString("f#/%1/%1.qts").arg(projectName);
}

QString FSharpGeneratorPlugin::extension() const
{
    return "smpl";
}

QString FSharpGeneratorPlugin::extDescrition() const
{
    return tr("F# Source File");
}

QString FSharpGeneratorPlugin::generatorName() const
{
    return "F#";
}
