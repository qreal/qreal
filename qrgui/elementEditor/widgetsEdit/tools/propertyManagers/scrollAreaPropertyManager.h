#pragma once

#include "../scrollArea.h"
#include "abstractScrollAreaPropertyManager.h"

namespace Ui
{
namespace WidgetsEdit
{

class ScrollAreaPropertyManager : public AbstractScrollAreaPropertyManager
{
public:
	ScrollAreaPropertyManager(ScrollArea *scrollArea);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QScrollArea *mScrollArea;
	QList<QPair<QString, int> > mProperties;

	void initProperties();
	bool containsProperty(QString const &name);

};

}
}
