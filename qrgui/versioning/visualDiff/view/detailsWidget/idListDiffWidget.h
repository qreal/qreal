#pragma once

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>

#include "idListWidget.h"
#include "../../model/diffModel.h"
#include "../../model/diffState.h"

namespace versioning
{
namespace details
{

class IdListDiffWidget : public QWidget
{
	Q_OBJECT

public:
	IdListDiffWidget(DiffModel *diffModel, QWidget *parent = 0);
	virtual ~IdListDiffWidget() {}

	void setIdLists(qReal::IdList const &same
			, qReal::IdList const &added
			, qReal::IdList const &removed);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);

protected:
	virtual void setLabelTitles();

	qReal::IdList mSameElements;
	qReal::IdList mAddedElements;
	qReal::IdList mRemovedElements;
	QLabel *mSameLabel;
	QLabel *mAddedLabel;
	QLabel *mRemovedLabel;

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);

private:
	DiffModel *mDiffModel;
	QGridLayout *mLayout;
	IdListWidget *mSameIdListWidget;
	IdListWidget *mAddedIdListWidget;
	IdListWidget *mRemovedIdListWidget;

};

}
}
