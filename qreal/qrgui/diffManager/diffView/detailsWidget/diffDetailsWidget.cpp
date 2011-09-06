#include "diffDetailsWidget.h"
#include "../../details/commonDifference.h"
#include <QScrollArea>

using namespace qReal::diffManager::diffView::detailsWidget;
using namespace qReal::diffManager::details;

DiffDetailsWidget::DiffDetailsWidget(details::DiffProvider *diffProvider, QWidget *parent)
	: QWidget(parent), mDiffProvider(diffProvider)
{
	initLayout();
	initIdWidget();
	initTabWidget();
	initParentWidget();
	initChildrenWidget();
	initPropertiesWidgets();
}

void DiffDetailsWidget::setId(const qReal::Id &graphicalId)
{
	if (graphicalId == mGraphicalId)
	{
		return;
	}
	closeSessionTabs();
	mGraphicalId = graphicalId;
	mLogicalId = mDiffProvider->logicalId(graphicalId);
	mIdWidget->setId(mGraphicalId, mLogicalId);
	mParentWidget->setId(mGraphicalId, mLogicalId);
	mChildrenWidget->setId(mGraphicalId, mLogicalId);
	mGraphicalPropertiesWidget->setId(mGraphicalId, mLogicalId);
	mLogicalPropertiesWidget->setId(mGraphicalId, mLogicalId);	
//	mTabWidget->setVisible(true);
}

void DiffDetailsWidget::reset()
{
	mGraphicalId = Id();
	mIdWidget->reset();
//	mTabWidget->setVisible(false);
	mParentWidget->reset();
	mChildrenWidget->reset();
	mGraphicalPropertiesWidget->reset();
	mLogicalPropertiesWidget->reset();
	closeSessionTabs();
}

void DiffDetailsWidget::initLayout()
{
	mLayout = new QGridLayout(this);
	mLayout->setContentsMargins(5, 5, 5, 5);
	mLayout->setColumnStretch(0, 10);
	mLayout->setRowStretch(0, 0);
	mLayout->setRowStretch(1, 10);
	setLayout(mLayout);
}

void DiffDetailsWidget::initIdWidget()
{
	mIdWidget = new IdWidget("<Click item to see details>", this);
	mLayout->addWidget(mIdWidget, 0, 0, Qt::AlignCenter);
	connect(mIdWidget, SIGNAL(mouseEntered(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mIdWidget, SIGNAL(mouseLeaved(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
}

void DiffDetailsWidget::initTabWidget()
{
	mTabWidget = new QTabWidget(this);
//	mTabWidget->setVisible(false);
	mLayout->addWidget(mTabWidget, 1, 0);
}

void DiffDetailsWidget::initParentWidget()
{
	mParentWidget = new ParentWidget(mDiffProvider, mTabWidget);
	connect(mParentWidget, SIGNAL(mouseEnteredIdWidget(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mParentWidget, SIGNAL(mouseLeavedIdWidget(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	openTab(mParentWidget, "Parents");
}

void DiffDetailsWidget::initChildrenWidget()
{
	mChildrenWidget = new ChildrenWidget(mDiffProvider, mTabWidget);
	connect(mChildrenWidget, SIGNAL(mouseEnteredIdWidget(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mChildrenWidget, SIGNAL(mouseLeavedIdWidget(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	openTab(mChildrenWidget, "Children");
}

void DiffDetailsWidget::initPropertiesWidgets()
{
	mGraphicalPropertiesWidget = new PropertiesWidget(mDiffProvider, true, this);
	mLogicalPropertiesWidget = new PropertiesWidget(mDiffProvider, false, this);
	connect(mGraphicalPropertiesWidget, SIGNAL(mouseEnteredIdWidget(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mGraphicalPropertiesWidget, SIGNAL(mouseLeavedIdWidget(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	connect(mGraphicalPropertiesWidget, SIGNAL(idListButtonClicked(bool,QString)), this, SLOT(onIdListButtonClicked(bool,QString)));
	connect(mLogicalPropertiesWidget, SIGNAL(mouseEnteredIdWidget(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(mLogicalPropertiesWidget, SIGNAL(mouseLeavedIdWidget(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	connect(mLogicalPropertiesWidget, SIGNAL(idListButtonClicked(bool,QString)), this, SLOT(onIdListButtonClicked(bool,QString)));
	openTab(mGraphicalPropertiesWidget, "Graphical Properties");
	openTab(mLogicalPropertiesWidget, "Logical Properties");
}

int DiffDetailsWidget::openTab(QWidget *widget, const QString &caption)
{
	for (int i = 0; i < mTabWidget->count(); ++i)
	{
		if (mTabWidget->tabText(i) == caption)
		{
			mTabWidget->setCurrentIndex(i);
			return i;
		}
	}

	QScrollArea *area = new QScrollArea(mTabWidget);
	area->setWidget(widget);
	area->setWidgetResizable(true);
	return mTabWidget->addTab(area, caption);
}

void DiffDetailsWidget::openNewIdListDiffTab(bool isGraphical, const QString &propertyName)
{
	IdListDiffWidget *widget = new IdListDiffWidget(mDiffProvider, this);
	connect(widget, SIGNAL(mouseEnteredIdWidget(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(widget, SIGNAL(mouseLeavedIdWidget(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	QString caption = propertyName + "(" + ((isGraphical) ? "graphical" : "logical") + ")";

	CommonDifference *difference;
	if (isGraphical)
		difference = mDiffProvider->difference(mGraphicalId)->graphicalDifference();
	else
		difference = mDiffProvider->difference(mLogicalId)->logicalDifference();
	if (Added == difference->propertiesState())
	{
		IdList added = difference->newProperty(propertyName).value<IdList>();
		widget->setIdLists(IdList(), added, IdList());
		mListDiffWidgets.append(widget);
		int index = openTab(widget, caption);
		mTabWidget->setCurrentIndex(index);
		return;
	}
	else
	{
		if (Removed == difference->propertiesState())
		{
			IdList removed = difference->oldProperty(propertyName).value<IdList>();
			widget->setIdLists(IdList(), IdList(), removed);
			mListDiffWidgets.append(widget);
			int index = openTab(widget, caption);
			mTabWidget->setCurrentIndex(index);
			return;
		}
	}
	if (Same == difference->propertyState(propertyName))
	{
		IdList same = difference->oldProperty(propertyName).value<IdList>();
		widget->setIdLists(same, IdList(), IdList());
		mListDiffWidgets.append(widget);
		int index = openTab(widget, caption);
		mTabWidget->setCurrentIndex(index);
		return;
	}
	IdList oldList = difference->oldProperty(propertyName).value<IdList>();
	IdList newList = difference->newProperty(propertyName).value<IdList>();
	IdList same;
	IdList added;
	IdList removed;
	foreach (Id id, oldList)
	{
		if (newList.contains(id))
		{
			same.append(id);
		}
		else
		{
			removed.append(id);
		}
	}
	foreach (Id id, newList)
	{
		if (!oldList.contains(id))
		{
			added.append(id);
		}
	}
	widget->setIdLists(same, added, removed);
	mListDiffWidgets.append(widget);
	int index = openTab(widget, caption);
	mTabWidget->setCurrentIndex(index);
}

void DiffDetailsWidget::closeSessionTabs()
{
	int sessionTabStart = 4;
	int tabCount = mTabWidget->count();
	for (int i = sessionTabStart; i < tabCount; ++i)
	{
		mTabWidget->removeTab(sessionTabStart);
	}
	foreach (IdListDiffWidget *widget, mListDiffWidgets)
	{
		widget->reset();
		delete widget;
	}
	mListDiffWidgets.clear();
}

void DiffDetailsWidget::onMouseEnteredIdWidget(const qReal::Id &id)
{
	emit mouseEnteredIdWidget(id);
}

void DiffDetailsWidget::onMouseLeavedIdWidget(const qReal::Id &id)
{
	emit mouseLeavedIdWidget(id);
}

void DiffDetailsWidget::onIdListButtonClicked(bool isGraphical, const QString &propertyName)
{
	openNewIdListDiffTab(isGraphical, propertyName);
}
