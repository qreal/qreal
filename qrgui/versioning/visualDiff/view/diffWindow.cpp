#include "diffWindow.h"

using namespace versioning;

DiffWindow::DiffWindow(qReal::MainWindow *mainWindow
		, DiffModel *diffModel, QWidget *parent)
	: QDialog(parent), mDiffModel(diffModel), mMainWindow(mainWindow)
{
	initWindow();
	initLayout();
	initButton();
	initViews();
	initDiffDetailsWidget();
	QList<int> sizes;
	sizes << 3 << 2;
	mSplitter->setSizes(sizes);
	mSplitter->setStretchFactor(0, 3);
	mSplitter->setStretchFactor(1, 2);
}

DiffWindow::~DiffWindow()
{
}

void DiffWindow::initWindow()
{
	setWindowTitle(tr("Diff"));
	setWindowState(Qt::WindowMaximized | Qt::WindowActive);
	setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint);
	setWindowOpacity(1.00);
}

void DiffWindow::initLayout()
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

void DiffWindow::initButton()
{
	mOkButton = new QPushButton;
	mOkButton->setText(tr("OK"));
	mOkButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
	connect(mOkButton, SIGNAL(clicked()), this, SLOT(accept()));
	mLayout->addWidget(mOkButton, 1, 0, Qt::AlignRight);
}

void DiffWindow::initViews()
{
	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

	mOldView = new details::DiffView(mMainWindow, mDiffModel, true, this);
	mNewView = new details::DiffView(mMainWindow, mDiffModel, false, this);
	QFrame *oldFrame = new QFrame(this);
	QFrame *newFrame = new QFrame(this);
	oldFrame->setLayout(initView(mOldView));
	newFrame->setLayout(initView(mNewView));

	QList<int> sizes;
	sizes << 1 << 1;

	splitter->addWidget(oldFrame);
	splitter->addWidget(newFrame);
	splitter->setSizes(sizes);
	mSplitter->addWidget(splitter);
}

QGridLayout *DiffWindow::initView(details::DiffView *view)
{
	QGridLayout *result = new QGridLayout(this);
	result->setColumnStretch(0, 10);
	result->setColumnStretch(1, 0);
	result->addWidget(view, 0, 0);
	QSlider *zoomSlider = new QSlider(Qt::Horizontal, this);
	zoomSlider->setMinimum(0);
	zoomSlider->setMaximum(100);
	zoomSlider->setValue(50);
	connect(zoomSlider, SIGNAL(valueChanged(int)), view, SLOT(adjustZoom(int)));
	view->adjustZoom(zoomSlider->value());
	result->addWidget(zoomSlider, 1, 0);
	return result;
}

void DiffWindow::initDiffDetailsWidget()
{
	mDiffDetailsWidget = new details::DiffDetailsWidget(mDiffModel, this);
	mSplitter->addWidget(mDiffDetailsWidget);
	mOldView->setDetailsWidget(mDiffDetailsWidget);
	mNewView->setDetailsWidget(mDiffDetailsWidget);
}
