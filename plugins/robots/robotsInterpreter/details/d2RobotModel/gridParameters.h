#pragma once

#include <QtWidgets/QSlider>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class GridParameters : public QFrame
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
};

}
}
}
}
}
