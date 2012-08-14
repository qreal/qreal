#pragma once

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>

#include "idWidget.h"
#include "../../model/commonDifference.h"
#include "../../model/diffModel.h"

namespace versioning
{
namespace details
{

class PropertyWidget : public QWidget
{
	Q_OBJECT

public:
	PropertyWidget(DiffModel *diffModel, bool isGraphical, QWidget *parent = 0);

	void setProperty(qReal::Id const &graphicalId, qReal::Id const &logicalId, QString const &propertyName);
	void reset();

signals:
	void mouseEnteredIdWidget(qReal::Id const& id);
	void mouseLeavedIdWidget(qReal::Id const& id);
	void idListButtonClicked(QString const &propertyName);

private slots:
	void onMouseEnteredIdWidget(qReal::Id const &id);
	void onMouseLeavedIdWidget(qReal::Id const &id);
	void onIdListButtonClicked();

private:
	details::CommonDifference *difference() const;
	qReal::Id id() const;
	void stretchLayout(int columnCount);
	void setPropertyValue(QLabel *label, IdWidget *idWidget, QVariant value, int column);
	void createIdWidget(IdWidget *idWidget, QVariant value, int column);
	void createButton();

	DiffModel *mDiffModel;
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

};

}
}
