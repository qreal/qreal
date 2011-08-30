#pragma once

#include "../../details/diffProvider.h"
#include "idWidget.h"
#include "parentWidget.h"
#include "childrenWidget.h"
#include "propertiesWidget.h"
#include "idListDiffWidget.h"

#include <QWidget>
#include <QGridLayout>
#include <QTabWidget>

namespace qReal
{
namespace diffManager
{
namespace diffView
{
namespace detailsWidget
{

class DiffDetailsWidget : public QWidget
{
	Q_OBJECT

public:
	DiffDetailsWidget(details::DiffProvider *diffProvider, QWidget *parent = 0);
	void setId(qReal::Id const &graphicalId);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const &id);
	void mouseLeavedIdWidget(qReal::Id const &id);

private:
	details::DiffProvider *mDiffProvider;
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

	void initLayout();
	void initIdWidget();
	void initTabWidget();
	void initParentWidget();
	void initChildrenWidget();
	void initPropertiesWidgets();
	int openTab(QWidget *widget, QString const &caption);
	void openNewIdListDiffTab(bool isGraphical, const QString &propertyName);
	void closeSessionTabs();

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);
	void onIdListButtonClicked(bool isGraphical, QString const &propertyName);

};

}
}
}
}
