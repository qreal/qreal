#pragma once

#include <QWidget>

#include "../../../../qrutils/graphicsUtils/painterInterface.h"
#include "../../../../qrutils/graphicsUtils/paintWidget.h"

namespace Ui
{
	class NxtDisplay;
}

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
class NxtDisplay : public QWidget
{
	Q_OBJECT
    
public:
	explicit NxtDisplay(QWidget *parent = 0);
	~NxtDisplay();
    
	bool leftButtonIsDown();
	bool rightButtonIsDown();
	bool centralButtonIsDown();
	bool bottomButtonIsDown();
	virtual void setPainter(graphicsUtils::PainterInterface *p);
	void repaintDisplay();
	int displayWidth();
	int displayHeight();

private:
	Ui::NxtDisplay *mUi;
};
}
}
}
}
