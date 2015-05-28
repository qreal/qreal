#include "kitBase/robotOutputWidget.h"

#include <QtWidgets/QScrollBar>

using namespace kitBase;

RobotOutputWidget::RobotOutputWidget(QWidget *parent)
	: utils::OutputWidget(parent)
{
	setWidget(&mOutput);
	mOutput.setReadOnly(true);
}

void RobotOutputWidget::print(const QString &text)
{
	mOutput.appendPlainText(text);
	mOutput.verticalScrollBar()->setValue(mOutput.verticalScrollBar()->maximum());
	emit showRequest();
}

void RobotOutputWidget::clear()
{
	mOutput.clear();
	emit hideRequest();
}

QString RobotOutputWidget::title() const
{
	return tr("Robot output");
}
