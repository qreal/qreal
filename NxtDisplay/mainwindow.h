#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    

private slots:
    void on_LeftButton_clicked();

    void on_CentralButton_clicked();

    void on_RightButton_clicked();

    void on_LowerButton_clicked();

signals:
    void Left();
    void Central();
    void Right();
    void Lower();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
