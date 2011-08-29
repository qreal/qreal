#pragma once

#include "idListWidget.h"
#include "../../details/diffProvider.h"
#include "../../details/diffState.h"

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

class IdListDiffWidget : public QWidget
{
	Q_OBJECT

public:
	IdListDiffWidget(details::DiffProvider *diffProvider, QWidget *parent = 0);
	void setIdLists(qReal::IdList const &same, qReal::IdList const &added, qReal::IdList const &removed);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);

protected:
	qReal::IdList mSameElements;
	qReal::IdList mAddedElements;
	qReal::IdList mRemovedElements;
	QLabel *mSameLabel;
	QLabel *mAddedLabel;
	QLabel *mRemovedLabel;
	virtual void setCaption();

private:
	details::DiffProvider *mDiffProvider;
	QGridLayout *mLayout;
	IdListWidget *mSameIdListWidget;
	IdListWidget *mAddedIdListWidget;
	IdListWidget *mRemovedIdListWidget;

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);

};

}
}
}
}
