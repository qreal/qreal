/** @file uml_nodeelementwidgetshelper.h
 * 	@brief class for working with widgets and layouts on item
 * */

#pragma once

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QWidget>
#include <QtGui/QGraphicsProxyWidget>

#include "../nodeElement.h"

namespace umlLib
{
namespace details
{

class NodeElementWidgetsHelper
{
public:
	NodeElementWidgetsHelper();

	void setNodeElement(NodeElement *element);
	void onResized();

private:
	void initWidget();
	void placeProxyWidget();

	NodeElement *mElement;
	QWidget *mWidget;
	QGraphicsProxyWidget *mProxyWidget;

};

}
}
