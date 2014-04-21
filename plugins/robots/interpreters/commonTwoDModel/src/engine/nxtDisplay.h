#pragma once

#include <QtWidgets/QWidget>

#include <qrutils/graphicsUtils/painterInterface.h>
#include <qrutils/graphicsUtils/paintWidget.h>

#include "commonTwoDModel/engine/twoDModelDisplayInterface.h"

namespace Ui
{
	class NxtDisplay;
}

namespace twoDModel {

class NxtDisplay : public QWidget, public engine::TwoDModelDisplayInterface
{
	Q_OBJECT

public:
	explicit NxtDisplay(QWidget *parent = 0);
	~NxtDisplay();

	virtual void setPainter(graphicsUtils::PainterInterface *p);

	bool leftButtonIsDown() const;
	bool rightButtonIsDown() const;
	bool centralButtonIsDown() const;
	bool bottomButtonIsDown() const;

	void repaintDisplay();
	int displayWidth() const;
	int displayHeight() const;

protected:
	void paintEvent(QPaintEvent *);

private:
	Ui::NxtDisplay *mUi;
	QImage mBackground;
};

}
