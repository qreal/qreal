#pragma once

#include "robotModel/parts/trikDisplay.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Display : public robotModel::parts::TrikDisplay
{
	Q_OBJECT

public:
	Display(interpreterBase::robotModel::DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port);

	void drawSmile(bool sad) override;
	/// @todo: color enum here?
	void setBackground(QColor const &color) override;
	void clearScreen() override;
	void printText(int x, int y, QString const &text) override;
	//void setPainterColor(QColor const &color) override;
	//void setPainterWidth(int penWidth) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
};

}
}
}
}
