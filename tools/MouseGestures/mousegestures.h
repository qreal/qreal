#pragma once
#include "GeometricForms.h"
#include "keyobjecttable.h"
#include <QtGui/QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>

namespace Ui
{
    class MouseGestures;
}

class MouseGestures : public QMainWindow
{
    Q_OBJECT

public:
    explicit MouseGestures(QWidget *parent = 0);
    ~MouseGestures();

private:
    Ui::MouseGestures *ui;
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void drawPath(QPainter * painter, QList<QPoint> const & path);
    void addEntities(EntityVector const & entities);
    QList<QPoint> mMousePath;
    QList<QPoint> mCorrectPath;
    KeyObjectTable mKeyObjectTable;
    static const int keyColumn = 2;
    static const int pathColumn = 1;
    static const int objectColumn = 0;

private slots:
    void loadFile();
    void changePath();
};
