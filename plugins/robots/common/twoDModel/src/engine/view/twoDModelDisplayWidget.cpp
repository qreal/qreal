#include "twoDModel/engine/twoDModelDisplayWidget.h"

using namespace twoDModel::engine;

TwoDModelDisplayWidget::TwoDModelDisplayWidget(QWidget *parent)
	: QWidget(parent)
	, mPainter(nullptr)
{
}

void TwoDModelDisplayWidget::setPainter(graphicsUtils::PainterInterface *painter)
{
	mPainter = painter;
}

void TwoDModelDisplayWidget::clear() const
{
	if (mPainter) {
		mPainter->clear();
	}
}
