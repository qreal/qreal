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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QAbstractScrollArea>
#include <QtWidgets/QStyle>
#include <QtCore/QEvent>
#include <QtGui/QResizeEvent>

#include "searchLineEdit.h"

using namespace qReal;
using namespace qReal::ui;

SearchLinePanel::SearchLinePanel(QWidget *parent)
	: QFrame(parent)
	, mSearchLineEdit(new SearchLineEdit(this, false))
	, mNextButton(new QPushButton(this))
	, mCloseButton(new QPushButton(this))
{
	auto hLayout = new QHBoxLayout(this);
	hLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
	hLayout->addWidget(mSearchLineEdit);
	mNextButton->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
	mNextButton->setFlat(true);
	mNextButton->setFixedWidth(mNextButton->height());
	hLayout->addWidget(mNextButton);
	mCloseButton->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
	mCloseButton->setFlat(true);
	mCloseButton->setFixedWidth(mCloseButton->height());

	hLayout->addWidget(mCloseButton);

	setFrameShape(QFrame::Box);
	hLayout->setContentsMargins(0, 0, 0, 0);
	hLayout->setMargin(0);
	hLayout->setSpacing(0);

	connect(mSearchLineEdit, &SearchLineEdit::textChanged, this, &SearchLinePanel::textChanged);
	connect(mNextButton, &QPushButton::pressed, this, &SearchLinePanel::nextPressed);
	connect(mCloseButton, &QPushButton::pressed, this, [this](){
		emit closePressed();
		detach();
		hide();
	});

	setBackgroundColor("white");
	hide();
}

void SearchLinePanel::setBackgroundColor(const QColor &color)
{
	setStyleSheet(QString("QFrame { background: %0; }").arg(color.name()));
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
	if (event->key() == Qt::Key_Escape) {
		mCloseButton->animateClick();
		event->accept();
	} else if (event->key() == Qt::Key_Return) {
		mNextButton->animateClick();
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
