/* Copyright 2007-2015 QReal Research Group
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

#include <QtGui/QMouseEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>

#include <qrutils/qRealDialog.h>

using namespace utils;

SmartDock::SmartDock(const QString &objectName, QWidget *innerWidget, QMainWindow *parent)
	: mMainWindow(parent ? parent : findMainWindow())
	, mInnerWidget(innerWidget)
	, mDialog(new QRealDialog(objectName, mMainWindow))
	, mCurrentMode(Mode::Docked)
{
	setObjectName(objectName);
	Q_ASSERT(mMainWindow);

	initDock();
	initDialog();

	mMainWindow->addDockWidget(Qt::RightDockWidgetArea, this);
}

SmartDock::~SmartDock()
{
}

void SmartDock::switchToDocked()
{
	if (mCurrentMode == Mode::Docked) {
		return;
	}

	mCurrentMode = Mode::Docked;
	mDialog->close();
	mDialog->layout()->removeWidget(mInnerWidget);
	setWidget(mInnerWidget);
	show();
}

void SmartDock::switchToFloating()
{
	if (mCurrentMode == Mode::Floats) {
		return;
	}

	mCurrentMode = Mode::Floats;
	setWidget(nullptr);
	close();
	static_cast<QVBoxLayout *>(mDialog->layout())->addWidget(mInnerWidget);
	mInnerWidget->show();
	mDialog->show();
}

void SmartDock::checkFloating()
{
	if (isFloating() && !mDragged) {
		switchToFloating();
	}
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
			checkFloating();
		}
		break;
	case QEvent::MouseButtonDblClick:
		mDragged = false;
		break;
	default:
		break;
	}

	return QDockWidget::event(event);
}

void SmartDock::initDock()
{
	setWindowTitle(mInnerWidget->windowTitle());
	setWidget(mInnerWidget);
	connect(this, &QDockWidget::topLevelChanged, this, &SmartDock::checkFloating);
}

void SmartDock::initDialog()
{
	mDialog->setWindowTitle(mInnerWidget->windowTitle());
	mDialog->setWindowFlags(mDialog->windowFlags() | Qt::WindowMinMaxButtonsHint);
	QVBoxLayout * const layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	mDialog->setLayout(layout);
	mDialog->setVisible(false);
}
