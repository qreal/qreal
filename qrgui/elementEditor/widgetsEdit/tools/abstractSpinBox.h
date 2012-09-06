#pragma once

#include <QtGui/QAbstractSpinBox>

#include "tool.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class AbstractSpinBox : public Tool
{
	Q_OBJECT

protected:
	AbstractSpinBox(QAbstractSpinBox *spinBox, ToolController *controller);

private:
	QAbstractSpinBox *mAbstractSpinBox;
};

}
}
