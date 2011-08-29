#pragma once

#include "../details/diffProvider.h"
#include "../../mainwindow/mainwindow.h"
#include "diffView.h"
#include "detailsWidget/diffDetailsWidget.h"

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

namespace qReal
{
namespace diffManager
{
namespace diffView
{

class DiffForm : public QDialog
{
	Q_OBJECT

public:
	explicit DiffForm(qReal::MainWindow *mainWindow, diffManager::details::DiffProvider *diffProvider, QWidget *parent = 0);
	~DiffForm();

private:
	diffManager::details::DiffProvider *mDiffProvider;
	QGridLayout *mLayout;
	QPushButton *mOkButton;
	DiffView *mOldView;
	DiffView *mNewView;
	detailsWidget::DiffDetailsWidget *mDiffDetailsWidget;
	MainWindow *mMainWindow;

	void initWindow();
	void initLayout();
	void initButton();
	void initViews();
	void initDiffDetailsWidget();
};

}
}
}


