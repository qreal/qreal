#include "searchLineEdit.h"

#include <QtCore/QPropertyAnimation>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QStyle>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLineEdit>

using namespace qReal::ui;

SearchLineEdit::SearchLineEdit(QWidget *parent)
	: QFrame(parent)
	, mOptionsButton(initButton(QIcon(":/widgets/icons/find.svg"), QString()))
	, mClearButton(initButton(style()->standardIcon(QStyle::SP_LineEditClearButton), tr("Clear text")))
	, mCaseInsensitive(new QAction(tr("Case insensitive"), this))
	, mCaseSensitive(new QAction(tr("Case sensitive"), this))
	, mRegularExpression(new QAction(tr("Regular expression"), this))
	, mCurrentOption(SearchOptions::CaseInsensitive)
{
	QHBoxLayout * const layout = new QHBoxLayout(this);
	layout->setContentsMargins(2, 2, 2, 2);
	layout->setSpacing(2);

	QLineEdit * const lineEdit = new QLineEdit(this);
	connect(lineEdit, &QLineEdit::textChanged, this, &SearchLineEdit::onTextChanged);
	lineEdit->setPlaceholderText(tr("Enter search text..."));
	lineEdit->setStyleSheet("border: 0");

	makeContextMenu();
	mOptionsButton->setPopupMode(QToolButton::InstantPopup);
	mOptionsButton->setFixedSize(32, 12);
	mCaseInsensitive->trigger();

	connect(mClearButton, &QAbstractButton::clicked, lineEdit, &QLineEdit::clear);
	mClearButton->setFixedSize(16, 16);
	mClearButton->hide();

	layout->addWidget(mOptionsButton);
	layout->addWidget(lineEdit);
	layout->addWidget(mClearButton);

	setStyleSheet("QFrame { background: white; border: 1px solid black; border-radius: 2px; }");
}

QToolButton *SearchLineEdit::initButton(const QIcon &icon, const QString &toolTip)
{
	QToolButton * const result = new QToolButton(this);
	result->setIcon(icon);
	result->setToolTip(toolTip);
	result->setStyleSheet("border: 0");
	return result;
}

void SearchLineEdit::onTextChanged(const QString &text)
{
	emit textChanged(regexpFromText(text, mCurrentOption));
	mClearButton->setVisible(!text.isEmpty());
}

void SearchLineEdit::makeContextMenu()
{
	connect(mCaseSensitive, &QAction::triggered, [=]() { mCurrentOption = SearchOptions::CaseSensitive; });
	connect(mCaseInsensitive, &QAction::triggered, [=]() { mCurrentOption = SearchOptions::CaseInsensitive; });
	connect(mRegularExpression, &QAction::triggered, [=]() { mCurrentOption = SearchOptions::RegularExpression; });

	QActionGroup * const group = new QActionGroup(this);
	group->setExclusive(true);
	group->addAction(mCaseInsensitive);
	group->addAction(mCaseSensitive);
	group->addAction(mRegularExpression);

	for (QAction * const action : group->actions()) {
		action->setCheckable(true);
	}

	QMenu * const menu = new QMenu(this);
	menu->addActions(group->actions());
	mOptionsButton->setMenu(menu);
}

QRegExp SearchLineEdit::regexpFromText(const QString &text, SearchOptions option) const
{
	if (option == SearchOptions::RegularExpression) {
		return QRegExp(text);
	}

	const QStringList parts = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	QRegExp result(parts.join("|"));
	result.setCaseSensitivity(option == SearchOptions::CaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
	return result;
}
