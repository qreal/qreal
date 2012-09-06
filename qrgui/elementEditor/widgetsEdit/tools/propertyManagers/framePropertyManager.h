#pragma once

#include "../frame.h"
#include "layoutToolPropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class FramePropertyManager : public LayoutToolPropertyManager
{
public:
	FramePropertyManager(Frame *frame);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QFrame *mFrame;
	QList<QPair<QString, int> > mProperties;
	QStringList mShapeTypes;
	QStringList mShadowTypes;

	void initProperties();
	bool containsProperty(QString const &name);

	void addEnum(QString const &name);

	QFrame::Shape shapeTypeToEnum(QString const &type);
	QString shapeEnumToType(QFrame::Shape shape);
	QFrame::Shadow shadowTypeToEnum(QString const &type);
	QString shadowEnumToType(QFrame::Shadow shape);
};

}
}
