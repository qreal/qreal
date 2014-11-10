#include "editorView.h"
#include "miniMapShell.h"

using namespace qReal;

int const buttonSize = 20;
int const miniMapStickDistance = 50;

MiniMapShell::MiniMapShell(QWidget *parent)
	: QWidget()
	, mParentWidget(parent)
	, mMiniMap(new MiniMap(parent))
	, mShowMiniMapButton(new MiniMapButton(this))
	, mMainLayout(new QVBoxLayout())
	, mAuxiliaryLayout(new QHBoxLayout())


{
	int const size = SettingsManager::value("MiniMapSize").toInt();

	mMiniMap->setParent(this);
	mMiniMap->show();
	mMiniMap->setGeometry(0, 0, size, size);

	mShowMiniMapButton->setParent(this);
	mShowMiniMapButton->setGeometry(0, 0, buttonSize, buttonSize);
	mShowMiniMapButton->raise();
	mShowMiniMapButton->setIcon(QIcon("://icons/show.png"));
	connect(mShowMiniMapButton, &MiniMapButton::clicked, this, &MiniMapShell::turnMiniMap);

	mMainLayout->addWidget(mMiniMap);
	setFixedSize(size + buttonSize, size + buttonSize);
	setLayout(mMainLayout);
	show();
	setParent(parent);
}

void MiniMapShell::changeSize(int const size)
{
	mMiniMap->setGeometry(0, 0, size, size);
	setFixedSize(size + buttonSize, size + buttonSize);
}

void MiniMapShell::turnMiniMap()
{
	if (!mShowMiniMapButton->getDragState()) {
		int const size = SettingsManager::value("MiniMapmSize").toInt();
		if (mMiniMap->isHidden()){
			mShowMiniMapButton->setIcon(QIcon("://icons/show.png"));
			mMiniMap->show();
			setGeometry(this->x()
						, this->y()
						, this->x() + size + buttonSize
						, this->y() + size + buttonSize);
		} else {
			mShowMiniMapButton->setIcon(QIcon("://icons/hide.png"));
			mMiniMap->hide();
			setGeometry(mShowMiniMapButton->geometry());
		}
	}

	mShowMiniMapButton->changeDragState(false);
}

void MiniMapShell::replace(QPoint const &position)
{
	QPoint pos = mParentWidget->mapFromGlobal(position);

	move(pos.x(), pos.y());

	int const editorViewHeight = mParentWidget->height();
	int const editorViewWidth = mParentWidget->width();

	if (pos.y() + height() > editorViewHeight - miniMapStickDistance) {
		move(pos.x(), editorViewHeight - height() - 10);
		if (pos.x() + width() > editorViewWidth - miniMapStickDistance) {
			pos = QPoint(editorViewWidth- width() - 10, editorViewHeight - height() - 10);
			move(pos);
		}

		if (pos.x() < miniMapStickDistance) {
			pos = QPoint(0, editorViewHeight - height() - 10);
			move(pos);
		}
	}

	if (pos.y() < miniMapStickDistance) {
		move(pos.x(), 0);
		if (pos.x() + width() > editorViewWidth - miniMapStickDistance) {
			pos = QPoint(editorViewWidth - width() - 10 , 0);
			move(pos);
		}

		if (pos.x() < miniMapStickDistance) {
			pos = QPoint(0, 0);
			move(pos);
		}
	}

	if (pos.x() + width() > editorViewWidth - miniMapStickDistance) {
		move(editorViewWidth - width() - 10, pos.y());
	}

	if (pos.x() < miniMapStickDistance) {
		move(0,pos.y());
	}
}

void MiniMapShell::adjustMinimapZoom(int zoom)
{
	mMiniMap->resetMatrix();
	mMiniMap->scale(0.01 * zoom, 0.01 * zoom);
}
