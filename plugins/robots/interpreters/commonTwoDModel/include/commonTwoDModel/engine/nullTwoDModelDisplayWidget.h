#pragma once

#include "twoDModelDisplayWidget.h"

namespace twoDModel {
namespace engine {

/// Display widget for NullRobotModel
class NullTwoDModelDisplayWidget : public twoDModel::engine::TwoDModelDisplayWidget
{
	Q_OBJECT

public:
	explicit NullTwoDModelDisplayWidget(QWidget *parent = 0);

	~NullTwoDModelDisplayWidget();

	void setPainter(graphicsUtils::PainterInterface *painter) override;
	bool buttonIsDown(const QString &buttonId) const override;
	void repaintDisplay();
	int displayWidth() const;
	int displayHeight() const;

};

}
}
