#include "childrenwidget.h"

using namespace qReal::diffManager::diffView::detailsWidget;
using namespace qReal::diffManager::details;

ChildrenWidget::ChildrenWidget(details::DiffProvider *diffProvider, QWidget *parent)
	: IdListDiffWidget(diffProvider, parent), mDiffProvider(diffProvider)
{
}

void ChildrenWidget::setId(const qReal::Id &graphicalId, const qReal::Id &logicalId)
{
	mGraphicalId = graphicalId;
	mLogicalId = logicalId;
	Difference *difference = mDiffProvider->difference(mLogicalId);
	if (!difference)
		return;
	IdList same;
	IdList added;
	IdList removed;
	qReal::IdList all = difference->graphicalDifference()->allChidren();
	foreach (qReal::Id id, all)
	{
		DiffState state = difference->graphicalDifference()->childState(id);
		switch(state)
		{
		case Same:
			same.append(id);
			break;
		case Added:
			added.append(id);
			break;
		case Removed:
			removed.append(id);
			break;
		default:
			qDebug() << "Modified or unknown child state";
			qDebug() << state;
		}
	}
	IdListDiffWidget::setIdLists(same, added, removed);
}

void ChildrenWidget::setCaption()
{
	if (mSameElements.empty() && mAddedElements.empty() && mRemovedElements.empty())
	{
		mSameLabel->setText("This element doesn`t have children");
		mAddedLabel->setText("");
		mRemovedLabel->setText("");
		return;
	}
	if (mSameElements.empty())
	{
		mSameLabel->setText("This element in the old model doesn`t have common children with the new models one.");
	}
	else
	{
		mSameLabel->setText("Children that element has in both models:");
	}
	if (mAddedElements.empty())
	{
		mAddedLabel->setText("This element doesn`t have children existing only in the new model.");
	}
	else
	{
		mAddedLabel->setText("Children existing only in the new model:");
	}
	if (mRemovedElements.empty())
	{
		mRemovedLabel->setText("This element doesn`t have children existing only in the old model.");
	}
	else
	{
		mRemovedLabel->setText("Children existing only in the old model:");
	}
}
