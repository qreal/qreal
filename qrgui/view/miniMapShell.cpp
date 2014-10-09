#include "editorView.h"
#include "miniMapShell.h"

using namespace qReal;

int const buttonSize = 20;

MiniMapShell::MiniMapShell(EditorView *parent, MiniMap * miniMap)
	: QWidget()
	, mEditorView(parent)
	, mMiniMap(miniMap)
	, mShowMiniMapButton(new MiniMapButton(parent))
	, mMainLayout(new QVBoxLayout())
	, mAuxiliaryLayout(new QHBoxLayout())
	, mSize(SettingsManager::value("MiniMapSize").toInt())
	, isMiniMapVisible(true)

{
	mMiniMap->setParent(this);
	mMiniMap->show();
	mShowMiniMapButton->setParent(this);

	setFixedSize(mSize + buttonSize, mSize + buttonSize);
	mMiniMap->setGeometry(0, 0, mSize, mSize);
	mShowMiniMapButton->setGeometry(0, 0, buttonSize, buttonSize);
	mShowMiniMapButton->raise();

	mMainLayout->addWidget(mMiniMap);
	this->setLayout(mMainLayout);

	connect(this->mShowMiniMapButton, SIGNAL(released()), this, SLOT(turnMiniMap()));

	mShowMiniMapButton->setIcon(QIcon(":/icons/show.png"));
}

void MiniMapShell::changeSize()
{
	mSize = SettingsManager::value("MiniMapmSize").toInt();
	mMiniMap->setGeometry(0, 0, mSize, mSize);
	setFixedSize(mSize + buttonSize, mSize + buttonSize);
}

void MiniMapShell::turnMiniMap()
{
	if (!mShowMiniMapButton->getDragState()) {
		mSize = SettingsManager::value("MiniMapmSize").toInt();
		if (isMiniMapVisible){
			mShowMiniMapButton->setIcon(QIcon(":/icons/show.png"));
			mMiniMap->show();
			setGeometry(this->x(), this->y(), this->x() + mSize + buttonSize, this->y() + mSize+buttonSize);
		} else {
			mShowMiniMapButton->setIcon(QIcon(":/icons/hide.png"));
			mMiniMap->hide();
			setGeometry(this->x(), this->y(), this->x() + buttonSize, this->y()+buttonSize);
		}

		isMiniMapVisible= !isMiniMapVisible;
		mShowMiniMapButton->changeDragState(false);
	} else {
		mShowMiniMapButton->changeDragState(false);
	}
}
