#pragma once

#include <QTreeWidget>
#include "../pluginManager/editorManagerInterface.h"

namespace qReal{
class PaletteTreeWidget : public QTreeWidget
{
public:
	PaletteTreeWidget(QWidget *parent, EditorManagerInterface *editorManagerProxy);
private:
	EditorManagerInterface *mEditorManagerProxy;
	void mousePressEvent(QMouseEvent *event);
};
}
