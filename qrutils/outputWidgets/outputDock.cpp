#include "outputDock.h"

using namespace utils;

OutputDock::OutputDock(QWidget *parent)
	: QDockWidget(parent)
{
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

	if (outputWidget == widget()) {
		hide();
	} else {
		showWidget(outputWidget);
	}
}

void OutputDock::showOutputWidget()
{
	showWidget(dynamic_cast<OutputWidget *>(sender()));
}

void OutputDock::showWidget(OutputWidget *widget)
{
	if (!widget) {
		return;
	}

	setWidget(widget);
	if (!isVisible()) {
		show();
	}
}

void OutputDock::hideOutputWidget()
{
	if (sender() == widget()) {
		hide();
	}
}
