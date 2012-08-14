#include "childrenWidget.h"

using namespace versioning;
using namespace versioning::details;

ChildrenWidget::ChildrenWidget(DiffModel *diffModel, QWidget *parent)
	: IdListDiffWidget(diffModel, parent), mDiffModel(diffModel)
{
}

void ChildrenWidget::setId(const qReal::Id &graphicalId, const qReal::Id &logicalId)
{
	mGraphicalId = graphicalId;
	mLogicalId = logicalId;
	Difference *difference = mDiffModel->difference(mLogicalId);
	if (!difference) {
		return;
	}
	qReal::IdList same;
	qReal::IdList added;
	qReal::IdList removed;
	qReal::IdList all = difference->graphicalDifference()->allChidren();
	foreach (qReal::Id const &id, all) {
		DiffState const state = difference->graphicalDifference()->childState(id);
		switch(state) {
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

void ChildrenWidget::setLabelTitles()
{
	if (mSameElements.empty() && mAddedElements.empty() && mRemovedElements.empty()) {
		mSameLabel->setText(tr("This element doesn`t have children"));
		mAddedLabel->setText("");
		mRemovedLabel->setText("");
		return;
	}

	if (mSameElements.empty()) {
		mSameLabel->setText(tr("1. This element in the old model doesn`t have common children with the new models one"));
	} else {
		mSameLabel->setText(tr("1. Children that element has in both models:"));
	}

	if (mAddedElements.empty()) {
		mAddedLabel->setText(tr("2. This element doesn`t have children existing only in the new model"));
	} else {
		mAddedLabel->setText(tr("2. Children existing only in the new model:"));
	}

	if (mRemovedElements.empty()){
		mRemovedLabel->setText(tr("3. This element doesn`t have children existing only in the old model."));
	} else {
		mRemovedLabel->setText(tr("3. Children existing only in the old model:"));
	}
}
