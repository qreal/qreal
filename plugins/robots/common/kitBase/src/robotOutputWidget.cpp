#include "kitBase/robotOutputWidget.h"

#include <QtWidgets/QScrollBar>

using namespace kitBase;

RobotOutputWidget::RobotOutputWidget(QWidget *parent)
	: utils::OutputWidget(parent)
	, mAction(tr("Show robot output"), nullptr)
{
	setWidget(&mOutput);
	mOutput.setReadOnly(true);

	mAction.setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));
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

QAction *RobotOutputWidget::action()
{
	return &mAction;
}

QString RobotOutputWidget::shortcutName() const
{
	return "Output.Robot";
}
