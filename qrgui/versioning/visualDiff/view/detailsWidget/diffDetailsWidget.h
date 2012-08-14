#pragma once

#include <QtGui/QWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QTabWidget>

#include "../../model/diffModel.h"
#include "idWidget.h"
#include "parentWidget.h"
#include "childrenWidget.h"
#include "propertiesWidget.h"
#include "idListDiffWidget.h"

namespace versioning
{
namespace details
{

class DiffDetailsWidget : public QWidget
{
	Q_OBJECT

public:
	DiffDetailsWidget(DiffModel *diffModel, QWidget *parent = 0);
	virtual ~DiffDetailsWidget() {}

	void setId(qReal::Id const &graphicalId);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const &id);
	void mouseLeavedIdWidget(qReal::Id const &id);

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);
	void onIdListButtonClicked(bool isGraphical, QString const &propertyName);

private:
	void initLayout();
	void initIdWidget();
	void initTabWidget();
	void initParentWidget();
	void initChildrenWidget();
	void initPropertiesWidgets();
	int openTab(QWidget *widget, QString const &caption);
	void openNewIdListDiffTab(bool isGraphical, const QString &propertyName);
	void closeSessionTabs();

	DiffModel *mDiffProvider;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;
	QGridLayout *mLayout;
	IdWidget *mIdWidget;
	QTabWidget *mTabWidget;
	ParentWidget *mParentWidget;
	ChildrenWidget *mChildrenWidget;
	PropertiesWidget *mGraphicalPropertiesWidget;
	PropertiesWidget *mLogicalPropertiesWidget;
	QList<IdListDiffWidget *> mListDiffWidgets;
};

}
}
