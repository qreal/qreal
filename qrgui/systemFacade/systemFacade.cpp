#include "systemFacade.h"

#include <qrgui/plugins/pluginManager/editorManager.h>

using namespace qReal;

SystemFacade::SystemFacade()
	: mEditorManager(new EditorManager)
	, mModels(QString(), mEditorManager)
{
	QObject::connect(&mModels.logicalModelAssistApi(), &models::LogicalModelAssistApi::elementAdded
			, &mEvents, &SystemEvents::logicalElementAdded);
	QObject::connect(&mModels.graphicalModelAssistApi(), &models::GraphicalModelAssistApi::elementAdded
			, &mEvents, &SystemEvents::graphicalElementAdded);
}

EditorManagerInterface &SystemFacade::editorManager()
{
	return mEditorManager;
}

models::Models &SystemFacade::models()
{
	return mModels;
}

SystemEvents &SystemFacade::events()
{
	return mEvents;
}
