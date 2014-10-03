#include "kitExtensionsUpdateManager.h"

using namespace interpreterCore;
using namespace generatorBase;

KitExtensionsUpdateManager::KitExtensionsUpdateManager(interpreterCore::KitPluginManager const &kitManager
		, TextManagerInterface &textManager
		, QObject *parent)
	: QObject(parent)
	, mKitManager(kitManager)
	, mTextManager(textManager)
{
}

void KitExtensionsUpdateManager::updateExtensions(interpreterBase::robotModel::RobotModelInterface &currentModel)
{
	mTextManager.removeExtensions();
	for (generatorBase::GeneratorKitPluginInterface *generator : mKitManager.generatorsById(currentModel.kitId())) {
		QString const extensionDescription = QString("%1 (*.%2)")
				.arg(generator->extensionDescription(), generator->extension());
		mTextManager.addExtensionDescriptionByExtension(generator->extension(), extensionDescription);
		mTextManager.addExtensionDescriptionByGenerator(generator->generatorName(), extensionDescription);
	}
}
