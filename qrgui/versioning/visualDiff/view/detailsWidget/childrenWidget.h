#pragma once

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>

#include "idListWidget.h"
#include "idListDiffWidget.h"
#include "../../model/diffModel.h"
#include "../../model/diffState.h"

namespace versioning
{
namespace details
{

class ChildrenWidget : public IdListDiffWidget
{
	Q_OBJECT

public:
	ChildrenWidget(DiffModel *diffProvider, QWidget *parent = 0);
	virtual ~ChildrenWidget() {}

	void setId(qReal::Id const &graphicalId, qReal::Id const &logicalId);

protected:
	virtual void setLabelTitles();

private:
	DiffModel *mDiffModel;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;
};

}
}
