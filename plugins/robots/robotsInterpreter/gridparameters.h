#pragma once
#include <QtWidgets/QSlider>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QObject>

class GridParameters
		: public QGroupBox
{
	Q_OBJECT

public:
	explicit GridParameters(QWidget *parent = 0);
	~GridParameters();

public slots:
	void showGrid(bool isGridEnabled);
	void setCellSize(int cellSizeValue);

signals:
	void parametersChanged();

private:
	QSlider *mCellSize;
	QCheckBox *mShowGridCheckBox;
	QLabel *mCellSizeLabel;
};

