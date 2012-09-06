#pragma once

#include "../abstractScrollArea.h"
#include "framePropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class AbstractScrollAreaPropertyManager : public FramePropertyManager
{
public:
	AbstractScrollAreaPropertyManager(AbstractScrollArea *abstractScrollArea);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	void initProperties();
	bool containsProperty(QString const &name) const;

	void addEnum(QString const &name);

	Qt::ScrollBarPolicy scrollBarPolicyTypeToEnum(QString const &type);
	QString scrollBarPolicyEnumToType(Qt::ScrollBarPolicy policy);

	QAbstractScrollArea *mAbstractScrollArea;
	QList<QPair<QString, int> > mProperties;
	QStringList mScrollBarPolicyTypes;

};

}
}
