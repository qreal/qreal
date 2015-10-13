/* Copyright 2007-2015 QReal Research Group
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

#include "styledButton.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QStyle>

#include "brandManager/brandManager.h"
#include "circleWidget.h"

using namespace qReal;

StyledButton::StyledButton(const QString &text, const QString &icon
		, QBoxLayout::Direction direction, QWidget *parent)
	: QPushButton(parent)
{
	setMouseTracking(true);

	QBoxLayout * const layout = new QBoxLayout(direction);
	layout->setMargin(2);
	if (!icon.isEmpty()) {
		QWidget * const circleWidget = new CircleWidget(QSize(70, 70), icon);
		layout->addWidget(circleWidget);
		bindHighlightedOnHover(circleWidget);
	}

	QLabel * const textLabel = new QLabel(text);
	textLabel->setWordWrap(true);
	textLabel->setAttribute(Qt::WA_Hover);
	layout->addWidget(textLabel);
	bindHighlightedOnHover(textLabel);

	setFlat(true);
	setStyleSheet(BrandManager::styles()->startTabButtonStyle());
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	setLayout(layout);
}

void StyledButton::bindHighlightedOnHover(QWidget * const widget)
{
	widget->setProperty("enabled", false);
	mChildren << widget;
}

void StyledButton::enterEvent(QEvent *event)
{
	QPushButton::enterEvent(event);
	highlight(true);
}

void StyledButton::leaveEvent(QEvent *event)
{
	QPushButton::leaveEvent(event);
	highlight(false);
}

void StyledButton::highlight(bool on)
{
	for (QWidget * const child : mChildren) {
		child->setProperty("enabled", on);
		child->style()->unpolish(child);
		child->style()->polish(child);
	}
}
