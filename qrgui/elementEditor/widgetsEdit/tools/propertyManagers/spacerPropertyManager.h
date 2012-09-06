#pragma once

#include "../spacer.h"
#include "toolPropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class SpacerPropertyManager : public ToolPropertyManager
{
public:
	SpacerPropertyManager(Spacer *spacer);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QWidget *mSpacer;
	QList<QPair<QString, int> > mProperties;

	void initProperties();
	bool containsProperty(QString const &name);

};

}
}
