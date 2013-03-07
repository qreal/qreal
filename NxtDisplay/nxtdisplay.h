#ifndef NXTDISPLAY_H
#define NXTDISPLAY_H

#include <QtGui/QWidget>

namespace Ui {
class NxtDisplay;
}

class NxtDisplay : public QWidget{
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
    void on_leftButton_clicked();
    void on_centralButton_clicked();
    void on_rightButton_clicked();
    void on_bottomButton_clicked();

private:
    Ui::NxtDisplay *ui;
};

#endif // NXTDISPLAY_H
