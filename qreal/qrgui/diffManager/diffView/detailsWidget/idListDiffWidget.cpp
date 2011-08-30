#include "idListDiffWidget.h"

using namespace qReal::diffManager::diffView::detailsWidget;
using namespace qReal::diffManager::details;

IdListDiffWidget::IdListDiffWidget(details::DiffProvider *diffProvider, QWidget *parent)
	: QWidget(parent), mSameElements(), mAddedElements(), mRemovedElements(), mDiffProvider(diffProvider)
{
	mLayout = new QGridLayout(this);
	mLayout->setMargin(3);

	mSameLabel = new QLabel(this);
	mAddedLabel = new QLabel(this);
	mRemovedLabel = new QLabel(this);

	QFont labelFont = mSameLabel->font();
	labelFont.setBold(true);
	mSameLabel->setFont(labelFont);
	mAddedLabel->setFont(labelFont);
	mRemovedLabel->setFont(labelFont);

	mSameIdListWidget = new IdListWidget(mDiffProvider, this);
	mAddedIdListWidget = new IdListWidget(mDiffProvider, this);
	mRemovedIdListWidget = new IdListWidget(mDiffProvider, this);

	connect(mSameIdListWidget, SIGNAL(mouseEnteredIdWidget(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mSameIdListWidget, SIGNAL(mouseLeavedIdWidget(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	connect(mAddedIdListWidget, SIGNAL(mouseEnteredIdWidget(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mAddedIdListWidget, SIGNAL(mouseLeavedIdWidget(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	connect(mRemovedIdListWidget, SIGNAL(mouseEnteredIdWidget(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mRemovedIdListWidget, SIGNAL(mouseLeavedIdWidget(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));

	mLayout->addWidget(mSameLabel, 0, 0);
	mLayout->addWidget(mSameIdListWidget, 1, 0);
	mLayout->addWidget(mAddedLabel, 2, 0);
	mLayout->addWidget(mAddedIdListWidget, 3, 0);
	mLayout->addWidget(mRemovedLabel, 4, 0);
	mLayout->addWidget(mRemovedIdListWidget, 5, 0);
}

void IdListDiffWidget::setIdLists(const qReal::IdList &same, const qReal::IdList &added, const qReal::IdList &removed)
{
	reset();
	mSameElements = same;
	mAddedElements = added;
	mRemovedElements = removed;
	setCaption();
	mSameIdListWidget->setIdList(mSameElements);
	mAddedIdListWidget->setIdList(mAddedElements);
	mRemovedIdListWidget->setIdList(mRemovedElements);
}

void IdListDiffWidget::reset()
{
	mSameLabel->setText("");
	mAddedLabel->setText("");
	mRemovedLabel->setText("");
	mSameElements.clear();
	mAddedElements.clear();
	mRemovedElements.clear();
	mSameIdListWidget->reset();
	mAddedIdListWidget->reset();
	mRemovedIdListWidget->reset();
}

void IdListDiffWidget::setCaption()
{
	if (mSameElements.empty() && mAddedElements.empty() && mRemovedElements.empty())
	{
		mSameLabel->setText("The list is empty");
		mAddedLabel->setText("");
		mRemovedLabel->setText("");
		return;
	}
	if (mSameElements.empty())
	{
		mSameLabel->setText("The list in the old model doesn`t contain common elements with the new models one.");
	}
	else
	{
		mSameLabel->setText("Elements that the list contains in both models:");
	}
	if (mAddedElements.empty())
	{
		mAddedLabel->setText("The list doesn`t contain elements existing only in the new model.");
	}
	else
	{
		mAddedLabel->setText("Elements that the  list contains only in the new model:");
	}
	if (mRemovedElements.empty())
	{
		mRemovedLabel->setText("The list doesn`t contain elements existing only in the old model.");
	}
	else
	{
		mRemovedLabel->setText("Elements that the  list contains only in the old model:");
	}
}

void IdListDiffWidget::onMouseEnteredIdWidget(const qReal::Id &id)
{
	emit mouseEnteredIdWidget(id);
}

void IdListDiffWidget::onMouseLeavedIdWidget(const qReal::Id &id)
{
	emit mouseLeavedIdWidget(id);
}
