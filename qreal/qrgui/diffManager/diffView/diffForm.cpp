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
	QList<int> sizes;
	sizes << 3 << 2;
	mSplitter->setSizes(sizes);
	mSplitter->setStretchFactor(0,3);
	mSplitter->setStretchFactor(1, 2);
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
	mLayout->setRowStretch(0, 10);
	mLayout->setRowStretch(1, 0);

	mSplitter = new QSplitter(Qt::Vertical, this);
	mSplitter->setFrameStyle(QFrame::Sunken);
	mLayout->addWidget(mSplitter);
	setLayout(mLayout);
}

void DiffForm::initButton()
{
	mOkButton = new QPushButton;
	mOkButton->setText("OK");
	mOkButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
	connect(mOkButton, SIGNAL(clicked()), this, SLOT(accept()));
	mLayout->addWidget(mOkButton, 1, 0, Qt::AlignRight);
}

void DiffForm::initViews()
{
	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	splitter->setFrameStyle(QFrame::Raised);

	mOldView = new DiffView(mMainWindow, mDiffProvider, true, this);
	mNewView = new DiffView(mMainWindow, mDiffProvider, false, this);	

	QList<int> sizes;
	sizes << 1 << 1;

	splitter->addWidget(mOldView);
	splitter->addWidget(mNewView);
	splitter->setSizes(sizes);
	mSplitter->addWidget(splitter);
}

void DiffForm::initDiffDetailsWidget()
{
	mDiffDetailsWidget = new DiffDetailsWidget(mDiffProvider, this);
	mSplitter->addWidget(mDiffDetailsWidget);
	mOldView->setDetailsWidget(mDiffDetailsWidget);
	mNewView->setDetailsWidget(mDiffDetailsWidget);
}
