#pragma once

#include <QtCore/QList>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>

#include "idWidget.h"
#include "../../model/diffModel.h"
#include "../../model/diffState.h"

namespace versioning
{
namespace details
{

class IdListWidget : public QWidget
{
	Q_OBJECT

public:
	IdListWidget(DiffModel *diffProvider, QWidget *parent = 0);

	void setIdList(qReal::IdList const &graphicalIdList);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);

private:
	void clearIdWidgets();
	void addIdWidget(int const index);

	DiffModel *mDiffProvider;
	qReal::IdList mGraphicalIdList;
	qReal::IdList mLogicalIdList;

	QGridLayout *mLayout;
	QList<IdWidget *> mIdWidgets;
};

}
}
