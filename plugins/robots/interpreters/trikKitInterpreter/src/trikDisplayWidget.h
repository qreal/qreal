#pragma once

#include <QtGui/QImage>

#include <qrutils/graphicsUtils/painterInterface.h>
#include <qrutils/graphicsUtils/paintWidget.h>

#include "commonTwoDModel/engine/twoDModelDisplayWidget.h"

namespace Ui {
class TrikDisplayWidget;
}

namespace trikKitInterpreter {

class TrikDisplayWidget : public twoDModel::engine::TwoDModelDisplayWidget
{
	Q_OBJECT

public:
	explicit TrikDisplayWidget(QWidget *parent = nullptr);
	~TrikDisplayWidget();

	void reset() override;

	void setPainter(graphicsUtils::PainterInterface *painter) override;

	void repaintDisplay() override;
	int displayWidth() const override;
	int displayHeight() const override;

	bool buttonIsDown(QString const &buttonId) const override;
	/// sets LED widget's color on 2D model display.
	void setLedColor(QColor const &color);

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void resetLed();

	Ui::TrikDisplayWidget *mUi;
	QImage mBackground;
};

}
