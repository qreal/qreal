/* Copyright 2017 CyberTech Labs Ltd.
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

#include "searchLinePanel.h"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QAbstractScrollArea>
#include <QtWidgets/QStyle>
#include <QtWidgets/QLineEdit>
#include <QtCore/QEvent>
#include <QtGui/QResizeEvent>
#include <QtCore/QPropertyAnimation>

#include "searchLineEdit.h"

using namespace qReal;
using namespace qReal::ui;

SearchLinePanel::SearchLinePanel(QWidget *parent)
	: QFrame(parent)
	, mSearchLineEdit(new SearchLineEdit(this, false))
	, mNextButton(new QPushButton(this))
	, mPreviousButton(new QPushButton(this))
	, mCloseButton(new QPushButton(this))
	, mReplaceButton(new QPushButton(this))
	, mReplaceLineEdit(new QLineEdit(this))
{
	auto gridLayout = new QGridLayout(this);
	auto hLayoutFst = new QHBoxLayout(this);
	auto hLayoutSnd = new QHBoxLayout(this);
	gridLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);

	hLayoutFst->addWidget(mSearchLineEdit);

	mNextButton->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
	mNextButton->setFlat(true);
	mNextButton->setFixedWidth(mNextButton->height());
	hLayoutFst->addWidget(mNextButton);

	mPreviousButton->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
	mPreviousButton->setFlat(true);
	mPreviousButton->setFixedWidth(mPreviousButton->height());
	hLayoutFst->addWidget(mPreviousButton);

	mCloseButton->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
	mCloseButton->setFlat(true);
	mCloseButton->setFixedWidth(mCloseButton->height());
	hLayoutFst->addWidget(mCloseButton);

	hLayoutSnd->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
	hLayoutSnd->addWidget(mReplaceLineEdit);
	mReplaceButton->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
	mReplaceButton->setFlat(true);
	mReplaceButton->setFixedWidth(mReplaceButton->height());
	hLayoutSnd->addWidget(mReplaceButton);

	gridLayout->setContentsMargins(0, 0, 0, 0);
	gridLayout->setMargin(0);
	gridLayout->setSpacing(0);

	gridLayout->addLayout(hLayoutFst, 0, 0);
	gridLayout->addLayout(hLayoutSnd, 1, 0);
	setFrameShape(QFrame::StyledPanel);
	setLayout(gridLayout);

	connect(mSearchLineEdit, &SearchLineEdit::textChanged, this, [this](const QRegExp &text){
		setSearchLineColor(QColor("white"));
		emit findTextChanged(text);
	});
	connect(mReplaceLineEdit, &QLineEdit::textChanged, this, &SearchLinePanel::replaceTextChanged);
	connect(mNextButton, &QPushButton::pressed, this, &SearchLinePanel::nextPressed);
	connect(mPreviousButton, &QPushButton::pressed, this, &SearchLinePanel::previousPressed);
	connect(mReplaceButton, &QPushButton::pressed, this, &SearchLinePanel::replacePressed);
    connect(mCloseButton, &QPushButton::pressed, this, [this](){
		emit closePressed();
		detach();
		hide();
	});

	setMode(OperationOptions::Find);
	hide();
}

void SearchLinePanel::reportError()
{
	QPropertyAnimation *animation = new QPropertyAnimation(this, "searchLineColor");
	animation->setDuration(300);
	animation->setStartValue(QColor(255, 0, 0, 0));
	animation->setKeyValueAt(0.5, QColor(255, 0, 0, 255));
	animation->setEndValue(QColor("white"));
	animation->start();
}

void SearchLinePanel::attachTo(QWidget *parent)
{
	detach();
	setParent(parent);
	parent->installEventFilter(this);
	show();
	moveAndAlign();

	mSearchLineEdit->focusMe();
}

void SearchLinePanel::detach()
{
	parent()->removeEventFilter(this);
}

void SearchLinePanel::setMode(const SearchLinePanel::OperationOptions &option)
{
	mCurrentOption = option;

	switch (option) {
		case SearchLinePanel::OperationOptions::Find: {
			mReplaceButton->hide();
			mReplaceLineEdit->hide();
			mPreviousButton->show();
			mSearchLineEdit->setSearchOption(SearchLineEdit::SearchOptions::CaseInsensitive);
			mSearchLineEdit->makeSearchOptionsSelectable(true);
			mSearchLineEdit->setPlaceHolderTextToLineEdit(tr("Enter search text..."));
			break;
		}

		case SearchLinePanel::OperationOptions::FindAndReplace: {
			mReplaceButton->show();
			mReplaceLineEdit->show();
			mPreviousButton->show();
			mSearchLineEdit->setSearchOption(SearchLineEdit::SearchOptions::CaseInsensitive);
			mSearchLineEdit->makeSearchOptionsSelectable(true);
			mSearchLineEdit->setPlaceHolderTextToLineEdit(tr("Enter search text..."));
			break;
		}

		case SearchLinePanel::OperationOptions::GoToLineAndColumn: {
			mReplaceButton->hide();
			mReplaceLineEdit->hide();
			mSearchLineEdit->clearText();
			mPreviousButton->hide();
			mSearchLineEdit->setSearchOption(SearchLineEdit::SearchOptions::CaseInsensitive);
			mSearchLineEdit->makeSearchOptionsSelectable(false);
			mSearchLineEdit->setPlaceHolderTextToLineEdit(tr("<line>:<column>"));
			break;
		}
	}
}

SearchLinePanel::OperationOptions SearchLinePanel::getMode() const
{
	return mCurrentOption;
}

QString SearchLinePanel::getTextForReplace() const
{
	return mReplaceLineEdit->text();
}

QString SearchLinePanel::getTextForFind() const
{
	return mSearchLineEdit->getText();
}

QColor SearchLinePanel::getSearchLineColor() const
{
	return mSearchLineColor;
}

void SearchLinePanel::setSearchLineColor(const QColor &color)
{
	mSearchLineColor = color;
	mSearchLineEdit->setLineEditColor(color);
}

bool SearchLinePanel::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::Resize) {
		auto resizeEvent = static_cast<QResizeEvent *>(event);
		move(resizeEvent->size().width() - width(), 0);
	}

	if (event->type() == QEvent::LayoutRequest) {
		moveAndAlign();
	}

	return QObject::eventFilter(obj, event);
}

void SearchLinePanel::keyPressEvent(QKeyEvent *event)
{
	if ((event->modifiers() & Qt::ShiftModifier) && event->key() == Qt::Key_Return) {
		mPreviousButton->animateClick();
		event->accept();
	} else if (event->key() == Qt::Key_Escape) {
		mCloseButton->animateClick();
		event->accept();
	} else if (not event->modifiers() && event->key() == Qt::Key_Return) {
		mNextButton->animateClick();
		event->accept();
	} else if (event->key() == Qt::Key_Tab && mReplaceLineEdit->isVisible()) {
		mReplaceLineEdit->setFocus(Qt::ShortcutFocusReason);
		event->accept();
	} else if (event->key() == Qt::Key_Backtab && mReplaceLineEdit->isVisible()) {
		mSearchLineEdit->focusMe();
		event->accept();
	} else {
		QFrame::keyPressEvent(event);
	}
}

void SearchLinePanel::moveAndAlign()
{
	if (QWidget *parent = parentWidget()) {
		auto scrollArea = dynamic_cast<QAbstractScrollArea *>(parent);
		if (scrollArea) {
			move(scrollArea->viewport()->width() - width(), 0);
		} else {
			move(parentWidget()->width() - width(), 0);
		}
	}
}
