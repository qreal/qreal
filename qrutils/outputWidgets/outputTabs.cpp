/* Copyright 2015 QReal Research Group
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

#include "outputTabs.h"

using namespace utils;

OutputTabs::OutputTabs(QWidget *parent)
	: QTabWidget(parent)
{
	setTabsClosable(true);
	connect(this, &OutputTabs::tabCloseRequested, this, &OutputTabs::closeTab);
}

OutputTabs::~OutputTabs()
{
	qDeleteAll(mWidgets);
}

void OutputTabs::addWidget(utils::OutputWidget *widget)
{
	mWidgets << widget;

	connect(widget, &OutputWidget::toggleVisibility, this, &OutputTabs::toggleVisibility);
	connect(widget, &OutputWidget::showRequest, this, &OutputTabs::showOutputWidget);
	connect(widget, &OutputWidget::hideRequest, this, &OutputTabs::hideOutputWidget);
}

void OutputTabs::toggleVisibility()
{
	OutputWidget *outputWidget = dynamic_cast<OutputWidget *>(sender());
	if (!outputWidget) {
		return;
	}

	if (outputWidget == currentWidget() && mDock->isVisible()) {
		mDock->hide();
	} else {
		showWidget(outputWidget);
	}
}

void OutputTabs::showOutputWidget()
{
	showWidget(dynamic_cast<OutputWidget *>(sender()));
}

void OutputTabs::showWidget(OutputWidget *outputWidget)
{
	if (!outputWidget) {
		return;
	}

	if (!mDock->isVisible()) {
		mDock->show();
	}

	for (int i = 0; i < count(); ++i) {
		if (outputWidget == widget(i)) {
			setCurrentIndex(i);
			return;
		}
	}

	setCurrentIndex(addTab(outputWidget, outputWidget->title()));
}

void OutputTabs::hideOutputWidget()
{
	if (sender() == currentWidget()) {
		closeTab(currentIndex());
	}
}

void OutputTabs::closeTab(int index)
{
	removeTab(index);
	if (count() == 0) {
		mDock->hide();
	}
}

void OutputTabs::setDock(QWidget *dock)
{
	mDock = dock;
}
