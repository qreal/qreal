#include "lineEdit.h"

using namespace Ui::WidgetsEdit;

LineEdit::LineEdit(ToolController *controller)
	: Tool(new QLineEdit, controller)
{
	mLineEdit = dynamic_cast<QLineEdit *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/lineEdit.png");
	mTitle = "Line Edit";
	connect(mLineEdit, SIGNAL(textChanged(QString))
		, this, SLOT(textChanged(QString)));
}

void LineEdit::textChanged(const QString &text)
{
	emit propertyChanged("text", QVariant(text));
}
