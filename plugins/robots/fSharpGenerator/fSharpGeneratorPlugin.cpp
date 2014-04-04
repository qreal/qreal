#include "fSharpGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

#include "fSharpMasterGenerator.h"

using namespace qReal;
using namespace qReal::robots::generators;
using namespace fSharp;

FSharpGeneratorPlugin::FSharpGeneratorPlugin()
		: mGenerateCodeAction(NULL)
{
    mAppTranslator.load(":/FSharpGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

FSharpGeneratorPlugin::~FSharpGeneratorPlugin()
{
}

QList<ActionInfo> FSharpGeneratorPlugin::actions()
{
    mGenerateCodeAction.setText(tr("Generate FSharp code"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));


    return {generateCodeActionInfo};
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
    return QString("FSharp/%1/%1.qts").arg(projectName);
}

QString FSharpGeneratorPlugin::extension() const
{
	return "qts";
}

QString FSharpGeneratorPlugin::extDescrition() const
{
    return tr("FSharp Source File");
}

QString FSharpGeneratorPlugin::generatorName() const
{
    return "FSharp";
}
