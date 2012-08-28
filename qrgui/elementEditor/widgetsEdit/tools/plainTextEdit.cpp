#include "plainTextEdit.h"

using namespace Ui::WidgetsEdit;

PlainTextEdit::PlainTextEdit(ToolController *controller)
	: AbstractScrollArea(new QPlainTextEdit, controller)
{
	mPlainTextEdit = dynamic_cast<QPlainTextEdit *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/plainTextEdit.png");
	mTitle = "Plain Text Edit";
	connect(mPlainTextEdit, SIGNAL(textChanged())
		, this, SLOT(textChanged()));
}

void PlainTextEdit::textChanged()
{
	emit propertyChanged("plainText", QVariant(mPlainTextEdit->toPlainText()));
}
