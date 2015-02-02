#include "runner.h"

using namespace twoDModel;

Runner::Runner()
	: mProjectManager(mQRealFacade.models())
	, mMainWindow(mErrorReporter, mQRealFacade.events()
			, mProjectManager, mQRealFacade.models().graphicalModelAssistApi())
	, mConfigurator(mQRealFacade.models().repoControlApi()
			, mQRealFacade.models().graphicalModelAssistApi()
			, mQRealFacade.models().logicalModelAssistApi()
			, mMainWindow
			, mProjectManager
			, mSceneCustomizer
			, mQRealFacade.events()
			, mTextManager)
{
	mPluginFacade.init(mConfigurator);
}

void Runner::interpret(const QString &saveFile)
{
}
