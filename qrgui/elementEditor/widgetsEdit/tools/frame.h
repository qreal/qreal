#pragma once

#include <QtGui/QFrame>

#include "layoutTool.h"

namespace qReal
{
namespace widgetsEdit
{

int const FRAME_DEFAULT_WIDTH = 150;
int const FRAME_DEFAULT_HEIGHT = 150;
int const FRAME_MINIMAL_WIDTH = 50;
int const FRAME_MINIMAL_HEIGHT = 50;

class Frame : public LayoutTool
{
public:
	Frame(ToolController *controller);

protected:
	Frame(QFrame *frame, ToolController *controller);

private:
	void initialize();

	QFrame *mFrame;

};

}
}
