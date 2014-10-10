#include "editorView.h"
#include "miniMapShell.h"

using namespace qReal;

int const buttonSize = 20;
int const miniMapStickDistance = 50;

MiniMapShell::MiniMapShell(QWidget *parent, MiniMap * miniMap)
	: QWidget()
	, mParentWidget(parent)
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

void MiniMapShell::replace(QPoint const pos)
{
	pos = mParentWidget->mapFromGlobal(pos);

	move(pos.x(), pos.y());

	int const editorViewHeight = mParentWidget.height();
	int const editorViewWidth = mParentWidget.width();

	if (pos.y() + height() > editorViewHeight - miniMapStickDistance) {
		move(pos.x(), editorViewHeight - height() - 10);
		if (pos.x() + width() > editorViewWidth - miniMapStickDistance) {
			QPoint const newPos = QPoint(editorViewWidth- width() - 10, editorViewHeight - height() - 10);
			move(newPos);
		}

		if (pos.x() < miniMapStickDistance) {
			QPoint const newPos = QPoint(0, editorViewHeight - height() - 10);
			move(newPos);
		}
	}

	if (pos.y() < miniMapStickDistance) {
		move(pos.x(), 0);
		if (pos.x() + width() > editorViewWidth - miniMapStickDistance) {
			QPoint const newPos = QPoint(editorViewWidth - width() - 10 , 0);
			move(newPos);
		}

		if (pos.x() < miniMapStickDistance) {
			QPoint const newPos = QPoint(0, 0);
			move(newPos);
		}
	}

	if (pos.x() + width() > editorViewWidth - miniMapStickDistance) {
		move(editorViewWidth - width() - 10, pos.y());
		if (pos.y() + height() > editorViewWidth - miniMapStickDistance) {
			QPoint const newPos = QPoint(editorViewWidth - width() - 10, editorViewHeight - height() - 10);
			move(newPos);
		}

		if (pos.y() < miniMapStickDistance) {
			QPoint const newPos = QPoint(editorViewWidth - width() - 10, 0);
			move(newPos);
		}
	}

	if (pos.x() < miniMapStickDistance) {
		move(0,pos.y());
		if (pos.y() + height() > editorViewWidth - miniMapStickDistance) {
			QPoint const newPos = QPoint(editorViewWidth - width() - 10 , 0);
			move(newPos);
		}
		if (pos.y() < miniMapStickDistance) {
			QPoint const newPos = QPoint(0, 0);
			move(newPos);
		}
	}
}
