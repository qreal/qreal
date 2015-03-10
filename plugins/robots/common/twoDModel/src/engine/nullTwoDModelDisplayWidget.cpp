#include "twoDModel/engine/nullTwoDModelDisplayWidget.h"

using namespace twoDModel::engine;

NullTwoDModelDisplayWidget::NullTwoDModelDisplayWidget(QWidget *parent)
	: twoDModel::engine::TwoDModelDisplayWidget(parent)
{
}

NullTwoDModelDisplayWidget::~NullTwoDModelDisplayWidget()
{
}

void NullTwoDModelDisplayWidget::setPainter(graphicsUtils::PainterInterface *painter)
{
	TwoDModelDisplayWidget::setPainter(painter);
}

bool NullTwoDModelDisplayWidget::buttonIsDown(const QString &buttonId) const
{
	Q_UNUSED(buttonId)
	return false;
}

void NullTwoDModelDisplayWidget::repaintDisplay()
{
}

int NullTwoDModelDisplayWidget::displayWidth() const
{
	return 0;
}

int NullTwoDModelDisplayWidget::displayHeight() const
{
	return 0;
}
