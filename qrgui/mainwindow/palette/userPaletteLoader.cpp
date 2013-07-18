#include "userPaletteLoader.h"

using namespace qReal::gui;

UserPaletteLoader::UserPaletteLoader(EditorManagerInterface const &editorManager
		, LogicalModelAssistInterface const *logicaApi)
	: mEditorManager(editorManager)
	, mLogicalApi(*logicaApi)
{
}

void UserPaletteLoader::loadPalette(PaletteTreeWidget &palette, Id const &diagram)
{
}
