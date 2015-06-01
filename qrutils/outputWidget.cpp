#include "outputWidget.h"

using namespace utils;

OutputWidget::OutputWidget(const QString &title, QWidget *parent)
	: QDockWidget(title, parent)
	, mAction(title, nullptr)
{
	connect(&mAction, &QAction::triggered, this, &OutputWidget::showMe);
}

QAction *OutputWidget::action()
{
	return &mAction;
}
