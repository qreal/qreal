#include "outputDock.h"

using namespace utils;

OutputDock::OutputDock(QWidget *parent)
	: QTabWidget(parent)
{
	setTabsClosable(true);
	connect(this, &OutputDock::tabCloseRequested, this, &OutputDock::closeTab);
}

OutputDock::~OutputDock()
{
	qDeleteAll(mWidgets);
}

void OutputDock::addWidget(utils::OutputWidget *widget)
{
	mWidgets << widget;

	connect(widget, &OutputWidget::toggleVisibility, this, &OutputDock::toggleVisibility);
	connect(widget, &OutputWidget::showRequest, this, &OutputDock::showOutputWidget);
	connect(widget, &OutputWidget::hideRequest, this, &OutputDock::hideOutputWidget);
}

void OutputDock::toggleVisibility()
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

void OutputDock::showOutputWidget()
{
	showWidget(dynamic_cast<OutputWidget *>(sender()));
}

void OutputDock::showWidget(OutputWidget *outputWidget)
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

void OutputDock::hideOutputWidget()
{
	if (sender() == currentWidget()) {
		closeTab(currentIndex());
	}
}

void OutputDock::closeTab(int index)
{
	removeTab(index);
	if (count() == 0) {
		mDock->hide();
	}
}

void OutputDock::setDock(QWidget *dock)
{
	mDock = dock;
}
