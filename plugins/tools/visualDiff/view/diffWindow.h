#pragma once

#include <QtWidgets/QSplitter>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

#include <qrgui/editor/sceneCustomizer.h>
#include <qrgui/controller/controller.h>

#include "model/diffModel.h"
#include "diffView.h"
#include "detailsWidget/diffDetailsWidget.h"


namespace versioning
{

class Q_DECL_EXPORT DiffWindow : public QWidget
{
	Q_OBJECT

public:
	DiffWindow(DiffModel *diffModel, bool compactMode, int diagram, QWidget *parent = 0);
	~DiffWindow();
	details::DiffView *getNewModel();
	bool diagramChanged();

private slots:
	void showDetails();

private:
	void initLayout();
	void initButton();
	void initViews();
	QGridLayout *initView(details::DiffView *view);
	void initDiffDetailsWidget();

	DiffModel *mDiffModel;
	QGridLayout *mLayout;
	QSplitter *mSplitter;
	QPushButton *mOkButton;
	QPushButton *mDetailsButton;
	details::DiffView *mOldView;
	details::DiffView *mNewView;
	details::DiffDetailsWidget *mDiffDetailsWidget;
	QWidget *mMainWindow;
	QLabel *mDetailsLabel;
	SceneCustomizer *mSceneCustomizer;
	Controller *mController;
	QTabWidget *mTabWidget;

	bool mShowDetails;
	bool mCompactMode;
	bool changed;
	int mDiagram;
};

}
