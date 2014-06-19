#pragma once

#include <QtWidgets/QGraphicsLinearLayout>
#include <qrgui/elementEditor/widgetsEdit/tools/labelTool.h>

#include "layoutTypes.h"
#include "layoutHelperBase.h"
#include "gridLayoutHelper.h"
#include "linearLayoutHelper.h"

namespace qReal
{
namespace widgetsEdit
{

class LayoutTool;
class GridLayoutHelper;
class LinearLayoutHelper;

class LayoutHelperFactory
{

public:
	LayoutHelperFactory(LayoutTool *tool);
	LayoutType currentType() const;
	void setLayout(const LayoutType type);

	static QString layoutTypeToString(const LayoutType type);

private:
	LayoutTool *mTool;

	LayoutType mType;

	GridLayoutHelper *mGridHelper;
	LinearLayoutHelper *mHorizontalHelper;
	LinearLayoutHelper *mVerticalHelper;

};

}
}
