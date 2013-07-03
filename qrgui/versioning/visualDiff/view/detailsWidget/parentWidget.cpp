#include "parentWidget.h"

using namespace versioning;
using namespace versioning::details;

ParentWidget::ParentWidget(DiffModel *diffProvider, QWidget *parent)
	: QWidget(parent), mDiffProvider(diffProvider)
{
	mLayout = new QGridLayout(this);
	mLayout->setMargin(5);
	mOldParentLabel = new QLabel(this);
	mNewParentLabel = new QLabel(this);
	mOldIdWidget = new IdWidget("", this);
	mNewIdWidget = new IdWidget("", this);
	connect(mOldIdWidget, SIGNAL(mouseEntered(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mOldIdWidget, SIGNAL(mouseLeaved(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	connect(mNewIdWidget, SIGNAL(mouseEntered(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mNewIdWidget, SIGNAL(mouseLeaved(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));

	QFont labelFont = mOldParentLabel->font();
	labelFont.setBold(true);
	mOldParentLabel->setFont(labelFont);
	mNewParentLabel->setFont(labelFont);

	mLayout->addWidget(mOldParentLabel, 0, 0);
	mLayout->addWidget(mOldIdWidget, 1, 0);
	mLayout->addWidget(mNewParentLabel, 2, 0);
	mLayout->addWidget(mNewIdWidget, 3, 0);
}

void ParentWidget::setId(const qReal::Id &graphicalId, const qReal::Id &logicalId)
{
	mGraphicalId = graphicalId;
	mLogicalId = logicalId;
	Difference *difference = mDiffProvider->difference(mLogicalId);
	if (!difference) {
		return;
	}
	DiffState const state = difference->graphicalDifference()->parentState();
	switch(state) {
	case Same:
		mOldParentLabel->setText(tr("Parents of this element in the old and new models are the same:"));
		mNewParentLabel->setText("");
		mOldIdWidget->setId(difference->graphicalDifference()->oldParent()
				, difference->logicalDifference()->oldParent());
		mNewIdWidget->reset();
		break;
	case Modified:
		mOldParentLabel->setText(tr("Parent in the old model:"));
		mNewParentLabel->setText(tr("Parent in the new model:"));
		mOldIdWidget->setId(difference->graphicalDifference()->oldParent()
				, difference->logicalDifference()->oldParent());
		mNewIdWidget->setId(difference->graphicalDifference()->newParent()
				, difference->logicalDifference()->newParent());
		break;
	case Added:
		mOldParentLabel->setText(tr("The element doesnt`t exist in the old model"));
		mNewParentLabel->setText(tr("Parent in the new model:"));
		mOldIdWidget->reset();
		mNewIdWidget->setId(difference->graphicalDifference()->newParent()
				, difference->logicalDifference()->newParent());
		break;
	case Removed:
		mOldParentLabel->setText(tr("Parent in the old model:"));
		mNewParentLabel->setText(tr("The element doesnt`t exist in the new model"));
		mOldIdWidget->setId(difference->graphicalDifference()->oldParent()
				, difference->logicalDifference()->oldParent());
		mNewIdWidget->reset();
		break;
	default:
		qDebug() << "Unknown Diff State";
		qDebug() << state;
		break;
	}
}

void ParentWidget::reset()
{
	mOldParentLabel->setText("");
	mNewParentLabel->setText("");
	mOldIdWidget->reset();
	mNewIdWidget->reset();
}

void ParentWidget::onMouseEnteredIdWidget(const qReal::Id &id)
{
	emit mouseEnteredIdWidget(id);
}

void ParentWidget::onMouseLeavedIdWidget(const qReal::Id &id)
{
	emit mouseLeavedIdWidget(id);
}
