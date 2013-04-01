#pragma once

#include <QtGui/QWidget>

namespace Ui
{
	class NxtDisplay;
}

class NxtDisplay : public QWidget
{
	Q_OBJECT
    
public:
	explicit NxtDisplay(QWidget *parent = 0);
	~NxtDisplay();
    
signals:
	void left();
	void right();
	void bottom();
	void central();

private slots:
	void onLeftButtonClicked();
	void onCentralButtonClicked();
	void onRightButtonClicked();
	void onBottomButtonClicked();

private:
	Ui::NxtDisplay *mUI;
};
