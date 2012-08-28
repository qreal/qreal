#pragma once

#include "../doubleSpinBox.h"
#include "abstractSpinBoxPropertyManager.h"

namespace Ui
{
namespace WidgetsEdit
{

class DoubleSpinBoxPropertyManager : public AbstractSpinBoxPropertyManager
{
public:
	DoubleSpinBoxPropertyManager(DoubleSpinBox *doubleSpinBox);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QDoubleSpinBox *mDoubleSpinBox;
	QList<QPair<QString, int> > mProperties;

	void initProperties();
	bool containsProperty(QString const &name);

};

}
}
