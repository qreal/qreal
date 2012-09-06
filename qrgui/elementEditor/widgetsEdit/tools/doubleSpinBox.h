#pragma once

#include <QtGui/QDoubleSpinBox>

#include "abstractSpinBox.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class DoubleSpinBox : public AbstractSpinBox
{
	Q_OBJECT

public:
	DoubleSpinBox(ToolController *controller);

private slots:
	void valueChanged(double d);

private:
	QDoubleSpinBox *mDoubleSpinBox;

};

}
}
