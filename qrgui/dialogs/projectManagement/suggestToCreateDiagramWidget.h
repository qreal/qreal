#pragma once

#include <qrkernel/ids.h>

#include "listWidget.h"

namespace qReal {

class EditorManagerInterface;

/// @brief ListWidget filled with available diagram types
/// Associated with item data are diagram Id
/// When you select an item class manually close the parent dialog
class SuggestToCreateDiagramWidget : public ListWidget
{
	Q_OBJECT

public:
	explicit SuggestToCreateDiagramWidget(const EditorManagerInterface &editorManager
			, QWidget *parent = 0);

private:
	void addItem(const Id &editor, const Id &diagram);

	const EditorManagerInterface &mEditorManager;
};

}
