#pragma once
#include <QtGui/QSlider>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QObject>

class gridParameters
        : public QGroupBox
{
    Q_OBJECT

public:
    explicit gridParameters(QWidget *parent = 0);
    ~gridParameters();

public slots:
    void showGrid(bool isGridEnabled);
    void setCellSize(int cellSizeValue);

signals:
    void parametersChanged();

private:
	QSlider *cellSize;
    QCheckBox *showGridCheckBox;
    QLabel *cellSizeLabel;
};

