#pragma once

#include "propertyManagerBase.h"

#include <QList>
#include <QPair>
#include <QVariant>

namespace Ui
{
namespace WidgetsEdit
{

class PropertyManagerBase;
class Tool;

class ToolPropertyManager : public PropertyManagerBase
{
public:
	ToolPropertyManager(Tool *tool);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QList<QPair<QString, int> > mProperties;

	void initProperties();
};

}
}
