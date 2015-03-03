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
