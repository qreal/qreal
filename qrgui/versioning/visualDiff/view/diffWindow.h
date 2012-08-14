#pragma once

#include <QtGui/QSplitter>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>

#include "../model/diffModel.h"
#include "diffView.h"
#include "detailsWidget/diffDetailsWidget.h"

namespace versioning
{

class DiffWindow : public QDialog
{
	Q_OBJECT

public:
	DiffWindow(qReal::MainWindow *mainWindow, DiffModel *diffModel, QWidget *parent = 0);
	~DiffWindow();

private:
	void initWindow();
	void initLayout();
	void initButton();
	void initViews();
	QGridLayout *initView(details::DiffView *view);
	void initDiffDetailsWidget();

	DiffModel *mDiffModel;
	QGridLayout *mLayout;
	QSplitter *mSplitter;
	QPushButton *mOkButton;
	details::DiffView *mOldView;
	details::DiffView *mNewView;
	details::DiffDetailsWidget *mDiffDetailsWidget;
	MainWindow *mMainWindow;
};

}
