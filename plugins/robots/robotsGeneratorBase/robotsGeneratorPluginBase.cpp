#include "robotsGeneratorPluginBase.h"

#include <qrutils/inFile.h>

using namespace qReal::robots::generators;

RobotsGeneratorPluginBase::RobotsGeneratorPluginBase()
{
	mAppTranslator.load(":/robotsGeneratorBase_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

void RobotsGeneratorPluginBase::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mRepo = dynamic_cast<qrRepo::RepoApi const *>(&configurator.logicalModelApi().logicalRepoApi());
	mProjectManager = &configurator.projectManager();
}

bool RobotsGeneratorPluginBase::generateCode()
{
	mProjectManager->save();
	mMainWindowInterface->errorReporter()->clearErrors();

	MasterGeneratorBase *generator = masterGenerator();
	generator->initialize();

	QString const generatedSrcPath = generator->generate();
	if (mMainWindowInterface->errorReporter()->wereErrors()) {
		delete generator;
		return false;
	}

	QString const generatedCode = utils::InFile::readAll(generatedSrcPath);
	if (!generatedCode.isEmpty()) {
		mMainWindowInterface->showInTextEditor(tr("Generated code"), generatedCode);
	}

	delete generator;
	return true;
}
