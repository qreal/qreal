#include "diffWindow.h"


using namespace versioning;

DiffWindow::DiffWindow(
		DiffModel *diffModel
		, bool compactMode
		, int diagram
		, QWidget *parent
		)
	: QWidget(parent)
	, mDiffModel(diffModel)
	, mMainWindow(parent)
	, mSceneCustomizer(new SceneCustomizer)
	, mController(new Controller)
	, mShowDetails(false)
	, mCompactMode(compactMode)
	, changed(false)
	, mDiagram(diagram)
{
	if (compactMode){
		this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
	} else{
		this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	}

	initLayout();
	initViews();
	initButton();
	initDiffDetailsWidget();
	QList<int> sizes;
	sizes << 5 << 2;
	mSplitter->setSizes(sizes);
	mSplitter->setStretchFactor(0, 5);
	mSplitter->setStretchFactor(1, 2);
}

DiffWindow::~DiffWindow()
{
}

versioning::details::DiffView *DiffWindow::getNewModel()
{
	return mNewView;
}

bool DiffWindow::diagramChanged()
{
	return changed;
}

void DiffWindow::showDetails()
{
	mShowDetails = !mShowDetails;
	mDiffDetailsWidget->setVisible(mShowDetails);
	if (mShowDetails){
		mDetailsLabel->setText(tr("click on details to HIDE it"));
	} else {
		mDetailsLabel->setText(tr("click on details to OPEN it"));
	}
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
	if (mCompactMode){
		mDetailsButton = new QPushButton;
		mDetailsButton->setText(tr("Details..."));
		mDetailsLabel = new QLabel(tr("click on details to OPEN it"));
		connect(mDetailsButton, SIGNAL(clicked()), this, SLOT(showDetails()));
		mLayout->addWidget(mDetailsButton, 1, 0, Qt::AlignRight);
		mLayout->addWidget(mDetailsLabel, 1, 0, Qt::AlignLeft);
	}
}

void DiffWindow::initViews()
{
	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	QList<int> sizes;
	sizes << 1;

	int oldDiagrams = mDiffModel->oldModel()->graphicalModelAssistApi().childrenOfRootDiagram();
	bool condition = mDiagram <= (oldDiagrams - 1);
	QModelIndex indexForOld = mDiffModel->oldModel()->graphicalModel()->index(condition ? mDiagram : 0, 0);
	Id rootIdForOld = mDiffModel->oldModel()->graphicalModelAssistApi().idByIndex(indexForOld);
	mOldView = new details::DiffView(mMainWindow, mDiffModel, true, *mController, *mSceneCustomizer, rootIdForOld);
	QFrame *oldFrame = new QFrame;
	oldFrame->setLayout(initView(mOldView));
	sizes << 1;
	splitter->addWidget(oldFrame);


	QModelIndex indexForNew = mDiffModel->newModel()->graphicalModel()->index(mDiagram,0);
	Id rootIdForNew = mDiffModel->newModel()->graphicalModelAssistApi().idByIndex(indexForNew);
	mNewView = new details::DiffView(mMainWindow, mDiffModel, false, *mController, *mSceneCustomizer, rootIdForNew);
	if (!mCompactMode && condition){
		QFrame *newFrame = new QFrame;
		newFrame->setLayout(initView(mNewView));
		splitter->addWidget(newFrame);
	} else {
		mNewView->setVisible(false);
	}

	splitter->setSizes(sizes);
	mSplitter->addWidget(splitter);
	changed = mNewView->isChanged() || mOldView->isChanged();
}

QGridLayout *DiffWindow::initView(details::DiffView *view)
{
	QGridLayout *result = new QGridLayout;
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
	if (mCompactMode){
		mDiffDetailsWidget->setVisible(mShowDetails);
	}
}
