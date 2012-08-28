#pragma once

#include "../pushButton.h"
#include "buttonPropertyManagerBase.h"

namespace Ui
{
namespace WidgetsEdit
{

class PushButtonPropertyManager : public ButtonPropertyManagerBase
{
public:
	PushButtonPropertyManager(PushButton *pushButton);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QPushButton *mPushButton;
	QList<QPair<QString, int> > mProperties;

	void initProperties();
	bool containsProperty(QString const &name);

};

}
}
