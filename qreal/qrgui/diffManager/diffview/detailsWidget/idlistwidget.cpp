#include "idlistwidget.h"

using namespace qReal::diffManager::diffView::detailsWidget;
using namespace qReal::diffManager::details;

IdListWidget::IdListWidget(DiffProvider *diffProvider, QWidget *parent)
	: QWidget(parent), mDiffProvider(diffProvider), mIdWidgets()
{
	mLayout = new QGridLayout(this);
	mLayout->setMargin(3);
}

void IdListWidget::setIdList(const qReal::IdList &graphicalIdList)
{
	reset();
	mGraphicalIdList = graphicalIdList;
	foreach (qReal::Id id, mGraphicalIdList)
	{
		mLogicalIdList.append(mDiffProvider->logicalId(id));
	}
	for (int i = 0; i<mGraphicalIdList.count(); ++i)
	{
		addIdWidget(i);
	}
}

void IdListWidget::reset()
{
	clearIdWidgets();
}

void IdListWidget::clearIdWidgets()
{
	foreach (IdWidget *idWidget, mIdWidgets)
	{
		mLayout->removeWidget(idWidget);
		delete idWidget;
	}
	mIdWidgets.clear();
	mGraphicalIdList.clear();
	mLogicalIdList.clear();
}

void IdListWidget::addIdWidget(int index)
{
	IdWidget *widget = new IdWidget("", this);
	widget->setId(mGraphicalIdList[index], mLogicalIdList[index]);
	mIdWidgets.append(widget);
	connect(widget, SIGNAL(mouseEntered(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(widget, SIGNAL(mouseLeaved(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	mLayout->addWidget(widget, index, 0);
}

void IdListWidget::onMouseEnteredIdWidget(const qReal::Id &id)
{
	emit mouseEnteredIdWidget(id);
}

void IdListWidget::onMouseLeavedIdWidget(const qReal::Id &id)
{
	emit mouseLeavedIdWidget(id);
}
