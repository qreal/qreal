#pragma once

#include "../../editorPluginInterface/widgetsHelperInterface.h"
#include "../nodeElement.h"

class WidgetsHelper : public WidgetsHelperInterface
{
public:
	WidgetsHelper(NodeElement *element);

	virtual bool initWidget(QString const &filename);

private:
	NodeElement *mElement;
	QWidget *mWidget;
};
