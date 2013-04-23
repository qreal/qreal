#pragma once

#include <QWidget>

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

private:
	Ui::NxtDisplay *mUi;
};
}
}
}
}
