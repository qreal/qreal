#include "kitExtensionsUpdateManager.h"

using namespace interpreterCore;
using namespace generatorBase;

KitExtensionsUpdateManager::KitExtensionsUpdateManager(interpreterCore::KitPluginManager const &kitManager
		, TextManagerInterface *textManager
		, QObject *parent)
	: QObject(parent)
	, mKitManager(kitManager)
	, mTextManager(textManager)
{
}

void KitExtensionsUpdateManager::updateExtensions(interpreterBase::robotModel::RobotModelInterface &currentModel)
{
	mTextManager->removeExtensions();
	for (generatorBase::GeneratorKitPluginInterface *generator : mKitManager.generatorsById(currentModel.kitId())) {
		QString const extDescr = QString("%1 (*.%2)")
				.arg(generator->extDescrition(), generator->extension());
		mTextManager->addExtDescrByExtension(generator->extension(), extDescr);
		mTextManager->addExtDescrByGenerator(generator->generatorName(), extDescr);
	}
}
