#include "searchLineEdit.h"

#include <QtCore/QPropertyAnimation>
#include <QtGui/QIcon>
#include <QtWidgets/QStyle>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLineEdit>

using namespace qReal::ui;

const QSize iconSize = QSize(16, 16);

SearchLineEdit::SearchLineEdit(QWidget *parent)
	: QFrame(parent)
{
	QHBoxLayout * const layout = new QHBoxLayout(this);
	layout->setContentsMargins(2, 2, 2, 2);
	layout->setSpacing(2);

	QLineEdit * const lineEdit = new QLineEdit(this);
	connect(lineEdit, &QLineEdit::textChanged, this, &SearchLineEdit::onTextChanged);
	lineEdit->setPlaceholderText(tr("Enter search text..."));
	lineEdit->setStyleSheet("border: 0");

	mActionsButton = initButton(QIcon(":/widgets/icons/find.svg"), QString());
	// mActionsButton will display options popup but it will serve as icon for a while.

	mClearButton = initButton(style()->standardIcon(QStyle::SP_LineEditClearButton)
			, tr("Clear text"));
	connect(mClearButton, &QAbstractButton::clicked, lineEdit, &QLineEdit::clear);
	mClearButton->hide();

	layout->addWidget(mActionsButton);
	layout->addWidget(lineEdit);
	layout->addWidget(mClearButton);

	setStyleSheet("QFrame { background: white; border: 1px solid black; border-radius: 2px; }");
}

QToolButton *SearchLineEdit::initButton(const QIcon &icon, const QString &toolTip)
{
	QToolButton * const result = new QToolButton(this);
	result->setFixedSize(16, 16);
	result->setIcon(icon);
	result->setToolTip(toolTip);
	result->setStyleSheet("border: 0");
	return result;
}

void SearchLineEdit::onTextChanged(const QString &text)
{
	emit textChanged(text);
	mClearButton->setVisible(!text.isEmpty());
}
