#pragma once

#include "propertywidget.h"
#include "../../details/diffprovider.h"
#include "../../details/diffstate.h"

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QList>

namespace qReal
{
namespace diffManager
{
namespace diffView
{
namespace detailsWidget
{

class PropertiesWidget : public QWidget
{
	Q_OBJECT

public:
	PropertiesWidget(details::DiffProvider *diffProvider, bool isGraphical, QWidget *parent = 0);
	void setId(qReal::Id const &graphicalId, qReal::Id const &logicalId);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);
	void idListButtonClicked(bool isGraphical, QString const &propertyName);

private:
	details::DiffProvider *mDiffProvider;
	bool mIsGraphical;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;

	QGridLayout *mLayout;
	QList<PropertyWidget *> mPropertyWidgets;

	details::CommonDifference *difference() const;
	void clearPropertyWidgets();
	void addPropertyWidget(QString const &propertyName, int index);

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);
	void onIdListButtonClicked(QString const &propertyName);

};

}
}
}
}
