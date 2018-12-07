/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

SearchLineEdit::SearchLineEdit(QWidget *parent, bool borderEnabled)
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

	mLineEdit = new QLineEdit(this);
	connect(mLineEdit, &QLineEdit::textChanged, this, &SearchLineEdit::onTextChanged);
	mLineEdit->setPlaceholderText(tr("Enter search text..."));
	mLineEdit->setStyleSheet("border: 0");

	makeContextMenu();
	mOptionsButton->setPopupMode(QToolButton::InstantPopup);
	mOptionsButton->setFixedSize(16, 12);
	mCaseInsensitive->trigger();

	connect(mClearButton, &QAbstractButton::clicked, mLineEdit, &QLineEdit::clear);
	mClearButton->setFixedSize(16, 16);
	mClearButton->hide();

	layout->addWidget(mOptionsButton);
	layout->addWidget(mLineEdit);
	layout->addWidget(mClearButton);

	setBorderEnabled(borderEnabled);
}

void SearchLineEdit::setBorderEnabled(bool enabled)
{
	if (enabled) {
		setStyleSheet("QFrame { background: white; border: 1px solid black; border-radius: 2px; }");
	} else {
		setStyleSheet("QFrame { background: white; }");
	}
}

void SearchLineEdit::setLineEditColor(const QColor &color)
{
	mLineEdit->setStyleSheet(QString("QLineEdit {background: %1}").arg(color.name(QColor::HexArgb)));
}

void SearchLineEdit::focusMe()
{
	mLineEdit->setFocus(Qt::ShortcutFocusReason);
}

void SearchLineEdit::setSearchOption(const SearchLineEdit::SearchOptions &option)
{
	mCurrentOption = option;
}

void SearchLineEdit::makeSearchOptionsSelectable(bool selectable)
{
	mOptionsButton->setEnabled(selectable);
}

void SearchLineEdit::setPlaceHolderTextToLineEdit(const QString &text)
{
	mLineEdit->setPlaceholderText(text);
}

QString SearchLineEdit::getText() const
{
	return mLineEdit->text();
}

void SearchLineEdit::clearText()
{
	mLineEdit->clear();
}

QToolButton *SearchLineEdit::initButton(const QIcon &icon, const QString &toolTip)
{
	QToolButton * const result = new QToolButton(this);
	result->setIcon(icon);
	result->setToolTip(toolTip);
	result->setStyleSheet("QToolButton { border: 0; } QToolButton:menu-indicator { image: none; }");
	return result;
}

void SearchLineEdit::onTextChanged(const QString &text)
{
	mClearButton->setVisible(!text.isEmpty());
	notifyTextChanged();
}

void SearchLineEdit::makeContextMenu()
{
	connect(mCaseSensitive, &QAction::triggered, this, [=]() {
		mCurrentOption = SearchOptions::CaseSensitive;
		notifyTextChanged();
	});

	connect(mCaseInsensitive, &QAction::triggered, this, [=]() {
		mCurrentOption = SearchOptions::CaseInsensitive;
		notifyTextChanged();
	});

	connect(mRegularExpression, &QAction::triggered, this, [=]() {
		mCurrentOption = SearchOptions::RegularExpression;
		notifyTextChanged();
	});

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

void SearchLineEdit::notifyTextChanged()
{
	emit textChanged(regexpFromText(mLineEdit->text(), mCurrentOption));
}

QRegExp SearchLineEdit::regexpFromText(const QString &text, SearchOptions option) const
{
	QRegExp result(text);

	switch (option) {
		case SearchOptions::RegularExpression:
			return result;
		case SearchOptions::CaseInsensitive:
			result.setPatternSyntax(QRegExp::FixedString);
			result.setCaseSensitivity(Qt::CaseInsensitive);
			return result;
		case SearchOptions::CaseSensitive:
			result.setPatternSyntax(QRegExp::FixedString);
			result.setCaseSensitivity(Qt::CaseSensitive);
			return result;
	}
}
