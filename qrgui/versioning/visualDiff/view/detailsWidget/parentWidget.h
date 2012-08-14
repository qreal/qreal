#pragma once

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

class ParentWidget : public QWidget
{
	Q_OBJECT

public:
	ParentWidget(DiffModel *diffProvider, QWidget *parent = 0);

	void setId(qReal::Id const &graphicalId, qReal::Id const &logicalId);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);

private:
	DiffModel *mDiffProvider;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;

	QGridLayout *mLayout;
	QLabel *mOldParentLabel;
	QLabel *mNewParentLabel;
	IdWidget *mOldIdWidget;
	IdWidget *mNewIdWidget;
};

}
}
