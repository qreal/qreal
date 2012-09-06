#pragma once

#include "../plainTextEdit.h"
#include "abstractScrollAreaPropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class PlainTextEditPropertyManager : public AbstractScrollAreaPropertyManager
{
public:
	PlainTextEditPropertyManager(PlainTextEdit *plainTextEdit);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QPlainTextEdit *mPlainTextEdit;
	QList<QPair<QString, int> > mProperties;

	void initProperties();
	bool containsProperty(QString const &name);

};

}
}
