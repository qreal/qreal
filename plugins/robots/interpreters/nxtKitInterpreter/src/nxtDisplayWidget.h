#pragma once

#include <QtWidgets/QWidget>

#include <qrutils/graphicsUtils/painterInterface.h>
#include <qrutils/graphicsUtils/paintWidget.h>

#include "commonTwoDModel/engine/twoDModelDisplayInterface.h"
#include "commonTwoDModel/engine/twoDModelDisplayWidget.h"

namespace Ui
{
	class NxtDisplay;
}

namespace nxtKitInterpreter {

class NxtDisplay : public twoDModel::engine::TwoDModelDisplayWidget
{
	Q_OBJECT

public:
	explicit NxtDisplay(QWidget *parent = 0);
	~NxtDisplay();

	virtual void setPainter(graphicsUtils::PainterInterface *p);

	/// @todo Supposes polling, which is not very good for buttons.
	bool buttonIsDown(QString const &buttonId) const override;

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
