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
};

}
}
}
}
