#include "guiFacade.h"

using namespace qReal;
using namespace gui;

GuiFacade::GuiFacade(MainWindow *mainWindow)
	: mMainWindow(mainWindow)
{
}

QWidget * GuiFacade::getWidgetRef(QString const &type, QString const &name, QString const &widgetName) const
{
	if (type == "Toolbar"){
		QList<QToolBar *> toolbarList = mMainWindow->findChildren<QToolBar *> ();
		for(int i=0; i<toolbarList.count(); i++)
		{
			QWidget *toolbar = toolbarList[i];
			if (toolbar->objectName()== name)
			{
				QList<QWidget *> widgetList = toolbar->findChildren<QWidget *> ();
				for(int i=0; i<widgetList.count(); i++)
				{
					QWidget *widget = widgetList[i];
					if (widget->objectName() == widgetName)
					{
						return widget;
					}
				}
			}
		}
	}
}
