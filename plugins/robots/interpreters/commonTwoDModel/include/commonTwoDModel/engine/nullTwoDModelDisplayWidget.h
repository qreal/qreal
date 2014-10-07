#pragma once

#include <QtWidgets/QWidget>

#include "twoDModelDisplayWidget.h"

namespace twoDModel {
namespace engine {

class NullTwoDModelDisplayWidget : public twoDModel::engine::TwoDModelDisplayWidget
{
	Q_OBJECT

public:
	explicit NullTwoDModelDisplayWidget(QWidget *parent = 0)
		: twoDModel::engine::TwoDModelDisplayWidget(parent)
	{
	}

	~NullTwoDModelDisplayWidget()
	{
	}

	void setPainter(graphicsUtils::PainterInterface *painter) override
	{
		TwoDModelDisplayWidget::setPainter(painter);
	}

	bool buttonIsDown(QString const &buttonId) const override
	{
		Q_UNUSED(buttonId)
	}

	void repaintDisplay()
	{
	}

	int displayWidth() const
	{
		return 0;
	}

	int displayHeight() const
	{
		return 0;
	}

};

}
}
