#pragma once

#include <QtGui/QLabel>

#include "frame.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

int const LABEL_DEFAULT_WIDTH = 50;
int const LABEL_DEFAULT_HEIGHT = 15;

class Label : public Frame
{
public:
	Label(ToolController *controller);

private:
	QLabel *mLabel;
};

}
}
