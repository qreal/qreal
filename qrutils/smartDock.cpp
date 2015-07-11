/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "smartDock.h"

#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QStyle>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include <qrutils/qRealDialog.h>

using namespace utils;

SmartDock::SmartDock(const QString &objectName, QWidget *innerWidget, QMainWindow *parent)
	: mMainWindow(parent ? parent : findMainWindow())
	, mInnerWidget(innerWidget)
	, mDialog(new QRealDialog(objectName))
	, mCurrentMode(Mode::Docked)
{
	setObjectName(objectName);

	initDock();
	initDialog();
}

SmartDock::~SmartDock()
{
	delete mDialog;
}

bool SmartDock::isCentral() const
{
	return !isFloating() && isVisible()
			// I know that const_cast is bad, but this is beacuse of bad qt design.
			&& mMainWindow->dockWidgetArea(const_cast<SmartDock *>(this)) == Qt::TopDockWidgetArea;
}

void SmartDock::switchToDocked()
{
	if (mCurrentMode == Mode::Docked) {
		return;
	}

	switchToDockedQuietly();
	show();
	checkCentralWidget();
	emit dockedChanged(true);
}

void SmartDock::switchToDockedQuietly()
{
	mCurrentMode = Mode::Docked;
	mDialog->close();
	mDialog->layout()->removeWidget(mInnerWidget);
	setWidget(mInnerWidget);
	setFloating(false);
}

void SmartDock::switchToFloating()
{
	if (mCurrentMode == Mode::Floats) {
		return;
	}

	switchToFloatingQuietly();
	mDialog->show();
	checkCentralWidget();
	emit dockedChanged(false);
}

void SmartDock::switchToFloatingQuietly()
{
	mCurrentMode = Mode::Floats;
	setWidget(nullptr);
	close();
	static_cast<QVBoxLayout *>(mDialog->layout())->addWidget(mInnerWidget);
	mInnerWidget->show();
}

void SmartDock::attachToMainWindow(Qt::DockWidgetArea area)
{
	if (!mMainWindow) {
		return;
	}

	mDialog->resumeSerialization();
	setParent(mMainWindow);
	mMainWindow->addDockWidget(area, this);
	if (mCurrentMode == Mode::Docked) {
		mCurrentMode = Mode::Floats;
		switchToDockedQuietly();
	} else {
		mCurrentMode = Mode::Docked;
		switchToFloatingQuietly();
	}
}

void SmartDock::detachFromMainWindow()
{
	mDialog->suspendSerialization();
	close();
	mDialog->close();
	if (mMainWindow) {
		mMainWindow->removeDockWidget(this);
		setParent(nullptr);
	}
}

void SmartDock::checkFloating()
{
	// Mouse button releasing may cause dock animation into some dock area.
	// topLevelChanged() will be emitted only when animation is finished, so
	// we must not change dock state when animation is running.
	if (isFloating() && !mDragged && !isAnimating()) {
		switchToFloating();
	}
}

void SmartDock::checkCentralWidget()
{
	const bool tabsVisible = isFloating() || !isVisible() || mMainWindow->dockWidgetArea(this) != Qt::TopDockWidgetArea;
	for (QTabWidget * const centralWidget : mMainWindow->centralWidget()->findChildren<QTabWidget *>()) {
		centralWidget->setVisible(tabsVisible);
	}

	mMainWindow->centralWidget()->setSizePolicy(QSizePolicy::Preferred
			, tabsVisible ? QSizePolicy::Preferred : QSizePolicy::Maximum);
}

bool SmartDock::isAnimating()
{
	// A bit hacky way to know dialog animation state. If user repositions dock
	if (!mMainWindow->isAnimated()) {
		return false;
	}

	for (QPropertyAnimation *animation : findChildren<QPropertyAnimation *>()) {
		if (animation->state() == QAbstractAnimation::Running && animation->propertyName() == "geometry") {
			return true;
		}
	}

	return false;
}

QMainWindow *SmartDock::findMainWindow() const
{
	for (QWidget * const topLevelWidget : QApplication::topLevelWidgets()) {
		if (QMainWindow * const window = dynamic_cast<QMainWindow *>(topLevelWidget)) {
			return window;
		}
	}

	return nullptr;
}

bool SmartDock::event(QEvent *event)
{
	switch (event->type()) {
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
		if (static_cast<QMouseEvent *>(event)->button() == Qt::LeftButton) {
			mDragged = event->type() == QEvent::MouseButtonPress;
			if (QEvent::MouseButtonRelease == event->type()) {
				// Mouse button releasing may cause dock animation into some dock area.
				// This animation is not started immediately, so checking for it when all handlers worked out.
				QTimer::singleShot(0, this, SLOT(checkFloating()));
			}
		}
		break;
	case QEvent::MouseButtonDblClick:
		mDragged = false;
		break;
	case QEvent::Show:
		if (!widget()) {
			close();
		}
		checkCentralWidget();
		break;
	case QEvent::Hide:
		checkCentralWidget();
		break;
	default:
		break;
	}

	return QDockWidget::event(event);
}

void SmartDock::initDock()
{
	if (!mMainWindow) {
		return;
	}

	setWindowTitle(mInnerWidget->windowTitle());
	setWidget(mInnerWidget);
	setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	connect(this, &QDockWidget::topLevelChanged, this, &SmartDock::checkFloating);
	connect(this, &QDockWidget::dockLocationChanged, this, &SmartDock::checkCentralWidget);
}

void SmartDock::initDialog()
{
	mDialog->setWindowTitle(mInnerWidget->windowTitle());
	mDialog->setWindowIcon(mInnerWidget->windowIcon());
	mDialog->setWindowFlags(mDialog->windowFlags() | Qt::WindowMinMaxButtonsHint);
	QVBoxLayout * const layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	mDialog->setLayout(layout);
	mDialog->setVisible(false);
	connect(mDialog, &QDialog::finished, [=]() {
		if (mMainWindow) {
			switchToDocked();
		} else {
			mInnerWidget->close();
		}
	});
}
