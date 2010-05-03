#pragma once
#include "GeometricForms.h"
#include "keyobjecttable.h"
#include "paintmanager.h"
#include <QtGui/QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QMenu>
#include <QAction>

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

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    Ui::MouseGestures *ui;
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void showTable();
    void paintEvent(QPaintEvent *);
    void addEntities(EntityVector const & entities);
    QList<QPoint> mMousePath;
    QList<QPoint> mCorrectPath;
    QString mFileName;
    KeyObjectTable mKeyObjectTable;
    PaintManager * mPaintManager;
    static const int keyColumn = 2;
    static const int pathColumn = 1;
    static const int objectColumn = 0;

private slots:
    void rotatePath();
    void increasePath();
    void loadFile();
    void changePath();
    void drawGesture();
    void save();
};
