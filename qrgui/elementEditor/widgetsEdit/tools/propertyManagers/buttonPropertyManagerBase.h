#pragma once

#include "../root.h"
#include "toolPropertyManager.h"

#include <QAbstractButton>

namespace Ui
{
namespace WidgetsEdit
{

class ButtonPropertyManagerBase : public ToolPropertyManager
{
public:
	ButtonPropertyManagerBase(Tool *buttonTool);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QAbstractButton *mButton;
	QList<QPair<QString, int> > mProperties;

	void initProperties();
	bool containsProperty(QString const &name);
};

}
}
