#pragma once

#include "idWidget.h"
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

class ParentWidget : public QWidget
{
	Q_OBJECT

public:
	ParentWidget(details::DiffProvider *diffProvider, QWidget *parent = 0);
	void setId(qReal::Id const &graphicalId, qReal::Id const &logicalId);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);

private:
	details::DiffProvider *mDiffProvider;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;

	QGridLayout *mLayout;
	QLabel *mOldParentLabel;
	QLabel *mNewParentLabel;
	IdWidget *mOldIdWidget;
	IdWidget *mNewIdWidget;

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);

};

}
}
}
}
