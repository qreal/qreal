#include "editorView.h"
#include "miniMapShell.h"

using namespace qReal;

MiniMapShell::MiniMapShell(EditorView *parent, MiniMap * miniMap)
	: QWidget()
	, mMainLayout(new QVBoxLayout())
	, mAuxiliaryLayout(new QHBoxLayout())
	, mMiniMap(miniMap)
	, mSize(SettingsManager::value("MiniMapSize").toInt())
	, isMiniMapVisible(true)
	, mShowMiniMapButton(new MiniMapButton(parent))
	, mEditorView(parent)
	, mButtonSize(20)

{
	mSize = SettingsManager::value("MiniMapSize").toInt();

	mMiniMap->setParent(this);
	mMiniMap->show();
	mShowMiniMapButton->setParent(this);

	setFixedSize(mSize + mButtonSize, mSize + mButtonSize);

	mMiniMap->setGeometry(0, 0, mSize, mSize);
	mShowMiniMapButton->setGeometry(0, 0, mButtonSize, mButtonSize);
	mShowMiniMapButton->raise();

	mMainLayout->addWidget(mMiniMap);

	this->setLayout(mMainLayout);

	connect(this->mShowMiniMapButton, SIGNAL(released()), this, SLOT(turnMiniMap()));

	mShowMiniMapButton->setIcon(QIcon(":/icons/show.png"));
}

void MiniMapShell::changeSize()
{
	mSize = SettingsManager::value("MiniMapmSize").toInt();
	setFixedSize(mSize, mSize);
	mMiniMap->setGeometry(0, 0, mSize, mSize);
	setFixedSize(mSize + mButtonSize, mSize + mButtonSize);
}

void MiniMapShell::currentTabChanged()
{
	mMiniMap->setScene(mEditorView->scene());
	mMiniMap->setParent(this);
	mShowMiniMapButton->raise();
}

void MiniMapShell::turnMiniMap()
{
	if (!mShowMiniMapButton->getDragState()) {
		mSize = SettingsManager::value("MiniMapmSize").toInt();
		if (isMiniMapVisible){
			mShowMiniMapButton->setIcon(QIcon(":/icons/show.png"));
			mMiniMap->show();
			setGeometry(this->x(), this->y(), this->x() + mSize + mButtonSize, this->y() + mSize+mButtonSize);
		} else {
			mShowMiniMapButton->setIcon(QIcon(":/icons/hide.png"));
			mMiniMap->hide();
			setGeometry(this->x(), this->y(), this->x() + mButtonSize, this->y()+mButtonSize);
		}

		isMiniMapVisible= !isMiniMapVisible;
		mShowMiniMapButton->changeDragState(false);
	} else {
		mShowMiniMapButton->changeDragState(false);
	}
}

QPoint const MiniMapShell::sceneCoordinates()
{
	return mSceneCoordinates;
}

void MiniMapShell::saveSceneCoordinates(QPoint const sceneCoordinates)
{
	mSceneCoordinates = sceneCoordinates;
}
