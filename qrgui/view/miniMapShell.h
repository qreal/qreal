#pragma once

#include <QtWidgets/QBoxLayout>

#include <qrkernel/settingsManager.h>
#include "mainwindow/miniMap.h"
#include "miniMapButton.h"

namespace qReal {

class MainWindow;
class EditorViewScene;
class EditorView;

class MiniMapShell : public QWidget
{
	Q_OBJECT

public:
	MiniMapShell(EditorView *parent, MiniMap *miniMap);

	void changeSize();
	void currentTabChanged();
	QPoint const sceneCoordinates();
public slots:
	void saveSceneCoordinates(QPoint const sceneCoordinates);

private slots:
	void turnMiniMap();

private:
	QVBoxLayout *mMainLayout;
	QHBoxLayout *mAuxiliaryLayout;
	MiniMapButton *mShowMiniMapButton;

	EditorView *mEditorView;
	MiniMap *mMiniMap;
	QPoint mSceneCoordinates;
	int mSize;
	int mButtonSize;
	bool isMiniMapVisible;

};

}
