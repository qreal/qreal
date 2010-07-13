#pragma once
#include <QWidget>

namespace Ui {
    class GesturesWindow;
}

class GesturesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GesturesWindow(QWidget *parent = 0);
    ~GesturesWindow();
    QString currentElement();
    void draw(QList<QPoint> idealPath);

private:
    Ui::GesturesWindow *ui;
    void clear();
};

