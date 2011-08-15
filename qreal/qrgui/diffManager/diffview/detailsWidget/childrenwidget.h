#pragma once

#include "idlistwidget.h"
#include "idlistdiffwidget.h"
#include "../../details/diffprovider.h"
#include "../../details/diffstate.h"

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

namespace qReal
{
namespace diffManager
{
namespace diffView
{
namespace detailsWidget
{

class ChildrenWidget : public IdListDiffWidget
{
	Q_OBJECT

public:
	ChildrenWidget(details::DiffProvider *diffProvider, QWidget *parent = 0);
	void setId(qReal::Id const &graphicalId, qReal::Id const &logicalId);

protected:
	virtual void setCaption();

private:
	details::DiffProvider *mDiffProvider;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;
};

}
}
}
}
