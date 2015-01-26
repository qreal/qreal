#include "shellWidget.h"

using namespace deployment;

ShellWidget::ShellWidget(QWidget *parent)
	: QTextEdit(parent)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Base, Qt::black);
	setPalette(pal);
	setTextColor(Qt::white);
	setReadOnly(true);
}
