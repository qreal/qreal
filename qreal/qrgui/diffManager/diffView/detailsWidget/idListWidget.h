#pragma once

#include "idWidget.h"
#include "../../details/diffProvider.h"
#include "../../details/diffState.h"

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

class IdListWidget : public QWidget
{
	Q_OBJECT

public:
	IdListWidget(details::DiffProvider *diffProvider, QWidget *parent = 0);
	void setIdList(qReal::IdList const &graphicalIdList);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);

private:
	details::DiffProvider *mDiffProvider;
	qReal::IdList mGraphicalIdList;
	qReal::IdList mLogicalIdList;

	QGridLayout *mLayout;
	QList<IdWidget *> mIdWidgets;

	void clearIdWidgets();
	void addIdWidget(int index);

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);

};

}
}
}
}
