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
	MiniMapShell(QWidget *parent, MiniMap *miniMap);

	void changeSize(int const size);

public slots:
	void replace(QPoint const &position);

private slots:
	void turnMiniMap();

private:
	QWidget *mParentWidget;
	MiniMap *mMiniMap;
	MiniMapButton *mShowMiniMapButton;

	QVBoxLayout *mMainLayout;
	QHBoxLayout *mAuxiliaryLayout;
};

}
