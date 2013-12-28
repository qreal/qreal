#include "styledButton.h"

#include <QtWidgets/QLabel>

#include "brandManager/brandManager.h"
#include "circleWidget.h"

using namespace qReal;

StyledButton::StyledButton(QString const &text, QString const &icon
		, QBoxLayout::Direction direction, QWidget *parent)
	: QPushButton(parent)
{
	setMouseTracking(true);

	QBoxLayout * const layout = new QBoxLayout(direction);
	if (!icon.isEmpty()) {
		QSize const iconSize(50, 40);
		QLabel * const iconLabel = new QLabel;
		iconLabel->setPixmap(QPixmap(icon));
		iconLabel->setScaledContents(true);
		iconLabel->setFixedSize(iconSize);

		QWidget * const circleWidget = new CircleWidget(QSize(70, 70), iconLabel);
		circleWidget->setProperty("enabled", false);
		layout->addWidget(circleWidget);
		mChildren << circleWidget;
	}

	QLabel * const textLabel = new QLabel(text);
	textLabel->setAttribute(Qt::WA_Hover);
	textLabel->setProperty("enabled", false);
	layout->addWidget(textLabel);
	mChildren << textLabel;

	setFlat(true);
	setStyleSheet(BrandManager::styles()->startTabButtonStyle());
	setFixedHeight(icon.isEmpty() ? 40 : 90);
	setLayout(layout);
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
