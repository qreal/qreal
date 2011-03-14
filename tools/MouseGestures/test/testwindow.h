#pragma once

#include <QMainWindow>
#include "abstractRecognizer.h"

namespace Ui {
    class TestWindow;
}

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();

private:
    Ui::TestWindow *ui;
    GesturesManager * getGesturesManager();

private slots:
    void test();
    void printData(int, int, int);
};

