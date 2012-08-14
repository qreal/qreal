#pragma once

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtCore/QList>

#include "propertyWidget.h"
#include "../../model/diffModel.h"
#include "../../model/diffState.h"

namespace versioning
{
namespace details
{

class PropertiesWidget : public QWidget
{
	Q_OBJECT

public:
	PropertiesWidget(DiffModel *diffModel, bool isGraphical, QWidget *parent = 0);

	void setId(qReal::Id const &graphicalId, qReal::Id const &logicalId);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);
	void idListButtonClicked(bool isGraphical, QString const &propertyName);

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);
	void onIdListButtonClicked(QString const &propertyName);

private:
	details::CommonDifference *difference() const;
	void clearPropertyWidgets();
	void addPropertyWidget(QString const &propertyName, int index);

	DiffModel *mDiffModel;
	bool mIsGraphical;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;

	QGridLayout *mLayout;
	QList<PropertyWidget *> mPropertyWidgets;
};

}
}
