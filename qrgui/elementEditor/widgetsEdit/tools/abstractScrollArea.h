#pragma once

#include <QtGui/QAbstractScrollArea>

#include "frame.h"

namespace Ui
{
namespace WidgetsEdit
{

class AbstractScrollArea : public Frame
{
protected:
	AbstractScrollArea(QAbstractScrollArea *area, ToolController *controller);

private:
	QAbstractScrollArea *mAbstractScrollArea;
};

}
}
