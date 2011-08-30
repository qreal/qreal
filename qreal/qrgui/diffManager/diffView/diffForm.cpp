#include "diffForm.h"
#include "../details/diffProvider.h"
#include "../../mainwindow/mainwindow.h"

using namespace qReal::diffManager::diffView;
using namespace qReal::diffManager::diffView::detailsWidget;
using namespace qReal::diffManager::details;

DiffForm::DiffForm(qReal::MainWindow *mainWindow, DiffProvider *diffProvider, QWidget *parent) :
		QDialog(parent), mDiffProvider(diffProvider), mMainWindow(mainWindow)
{
	initWindow();
	initLayout();
	initButton();
	initViews();
	initDiffDetailsWidget();
}

DiffForm::~DiffForm()
{
}

void DiffForm::initWindow()
{
	setWindowTitle("Diff");
	setWindowState(Qt::WindowMaximized | Qt::WindowActive);
	setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint);
	setWindowOpacity(1.00);
}

void DiffForm::initLayout()
{
	mLayout = new QGridLayout(this);

	mLayout->setContentsMargins(5, 5, 5, 5);
	mLayout->setColumnStretch(0, 10);
	mLayout->setRowStretch(0, 30);
	mLayout->setRowStretch(1, 10);
	mLayout->setRowStretch(2, 0);
	setLayout(mLayout);
}

void DiffForm::initButton()
{
	mOkButton = new QPushButton;
	mOkButton->setText("OK");
	mOkButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
	connect(mOkButton, SIGNAL(clicked()), this, SLOT(accept()));
	mLayout->addWidget(mOkButton, 2, 0, Qt::AlignRight);
}

void DiffForm::initViews()
{
	QGridLayout *viewsLayout = new QGridLayout(this);
	viewsLayout->setColumnStretch(0, 10);
	viewsLayout->setColumnStretch(1, 10);
	viewsLayout->setRowStretch(0, 10);
	viewsLayout->setContentsMargins(5, 5, 5, 5);

	mOldView = new DiffView(mMainWindow, mDiffProvider, true, this);
	mNewView = new DiffView(mMainWindow, mDiffProvider, false, this);	
	viewsLayout->addWidget(mOldView, 0, 0);
	viewsLayout->addWidget(mNewView, 0, 1);
	mLayout->addLayout(viewsLayout, 0, 0);
}

void DiffForm::initDiffDetailsWidget()
{
	mDiffDetailsWidget = new DiffDetailsWidget(mDiffProvider, this);
	mLayout->addWidget(mDiffDetailsWidget, 1, 0);
	mOldView->setDetailsWidget(mDiffDetailsWidget);
	mNewView->setDetailsWidget(mDiffDetailsWidget);
}
