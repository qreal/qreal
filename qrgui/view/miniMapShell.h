#pragma once

#include <QtWidgets/QBoxLayout>

#include <qrkernel/settingsManager.h>
#include "miniMap.h"
#include "miniMapButton.h"

namespace qReal {

class EditorView;

class MiniMapShell : public QWidget
{
	Q_OBJECT

public:
	MiniMapShell(EditorView *parent, MiniMap *miniMap);

	void changeSize();

private slots:
	void turnMiniMap();

private:
	EditorView *mEditorView;
	MiniMap *mMiniMap;
	MiniMapButton *mShowMiniMapButton;

	QVBoxLayout *mMainLayout;
	QHBoxLayout *mAuxiliaryLayout;

	int mSize;
	bool isMiniMapVisible;
};

}
