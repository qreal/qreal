#pragma once

#include "idwidget.h"
#include "../../details/commondifference.h"
#include "../../details/diffprovider.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

namespace qReal
{
namespace diffManager
{
namespace diffView
{
namespace detailsWidget
{

class PropertyWidget : public QWidget
{
	Q_OBJECT

public:
	PropertyWidget(details::DiffProvider *diffProvider, bool isGraphical, QWidget *parent = 0);
	void setProperty(qReal::Id const &graphicalId, qReal::Id const &logicalId, QString const &propertyName);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);
	void idListButtonClicked(QString const &propertyName);

private:
	details::DiffProvider *mDiffProvider;
	bool mIsGraphical;
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;
	QString mPropertyName;

	QGridLayout *mLayout;
	QLabel *mPropertyNameLabel;
	QLabel *mOldValueLabel;
	QLabel *mNewValueLabel;
	IdWidget *mOldIdWidget;
	IdWidget *mNewIdWidget;
	QPushButton *mIdListButton;

	details::CommonDifference *difference() const;
	qReal::Id id() const;
	void stretchLayout(int columnCount);
	void setPropertyValue(QLabel *label, IdWidget *idWidget, QVariant value, int column);
	void createIdWidget(IdWidget *idWidget, QVariant value, int column);
	void createButton();

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);
	void onIdListButtonClicked();

};

}
}
}
}
