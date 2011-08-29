#include "propertyWidget.h"
#include "../../details/diffState.h"

using namespace qReal::diffManager::diffView::detailsWidget;
using namespace qReal::diffManager::details;

PropertyWidget::PropertyWidget(DiffProvider *diffProvider, bool isGraphical, QWidget *parent)
	: QWidget(parent), mDiffProvider(diffProvider), mIsGraphical(isGraphical)
{
	mLayout = new QGridLayout(this);
	mLayout->setMargin(3);
	mPropertyNameLabel = new QLabel(this);
	mLayout->addWidget(mPropertyNameLabel, 0, 0);
}

void PropertyWidget::setProperty(const qReal::Id &graphicalId, const qReal::Id &logicalId, const QString &propertyName)
{
	mGraphicalId = graphicalId;
	mLogicalId = logicalId;
	mPropertyName = propertyName;
	mPropertyNameLabel->setText(propertyName + ": ");

	DiffState state = difference()->propertiesState();
	if (Added == state)
	{
		stretchLayout(1);
		setPropertyValue(mNewValueLabel, mNewIdWidget, difference()->newProperty(mPropertyName), 1);
		return;
	}
	else
	{
		if (Removed == state)
		{
			stretchLayout(1);
			setPropertyValue(mOldValueLabel, mOldIdWidget, difference()->oldProperty(mPropertyName), 1);
			return;
		}
	}
	state = difference()->propertyState(propertyName);
	if (Same == state)
	{
		stretchLayout(1);
		setPropertyValue(mOldValueLabel, mOldIdWidget, difference()->oldProperty(mPropertyName), 1);
	}
	else
	{
		stretchLayout(2);
		setPropertyValue(mOldValueLabel, mOldIdWidget, difference()->oldProperty(mPropertyName), 1);
		setPropertyValue(mNewValueLabel, mNewIdWidget, difference()->newProperty(mPropertyName), 2);
	}
}

void PropertyWidget::reset()
{
	mPropertyNameLabel->setText("");
}

CommonDifference *PropertyWidget::difference() const
{
	CommonDifference *result;
	if (mIsGraphical)
		result = mDiffProvider->difference(mGraphicalId)->graphicalDifference();
	else
		result = mDiffProvider->difference(mLogicalId)->logicalDifference();
	return result;
}

qReal::Id PropertyWidget::id() const
{
	return (mIsGraphical) ? mGraphicalId : mLogicalId;
}

void PropertyWidget::stretchLayout(int columnCount)
{
	mLayout->setRowStretch(0, 10);
	if (1 == columnCount)
	{
		mLayout->setRowStretch(1, 40);
	}
	else
	{
		mLayout->setRowStretch(1, 20);
		mLayout->setRowStretch(2, 20);
	}
}

void PropertyWidget::setPropertyValue(QLabel *label, IdWidget *idWidget, QVariant value, int column)
{
	if (value.type() == QVariant::UserType)
	{
		if (value.userType() == QMetaType::type("qReal::Id"))
		{
			createIdWidget(idWidget, value, column);
			return;
		}
		if (value.userType() == QMetaType::type("qReal::IdList"))
		{
			createButton();
			return;
		}
		qDebug() << "Unsupported QVariant type.";
		qDebug() << value;
		return;
	}
	label = new QLabel(this);
	label->setText(value.toString());
	mLayout->addWidget(label, 0, column);
}

void PropertyWidget::createIdWidget(IdWidget *idWidget, QVariant value, int column)
{
	idWidget = new IdWidget("", this);
	Id id = value.value<Id>();
	if (mIsGraphical)
	{
		idWidget->setId(id, Id());
	}
	else
	{
		idWidget->setId(Id(), id);
	}
	connect(idWidget, SIGNAL(mouseEntered(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(idWidget, SIGNAL(mouseLeaved(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	mLayout->addWidget(idWidget, 0, column);
}

void PropertyWidget::createButton()
{
	mIdListButton = new QPushButton(this);
	mIdListButton->setText("View id list difference");
	connect(mIdListButton, SIGNAL(clicked()), this, SLOT(onIdListButtonClicked()));
	stretchLayout(1);
	mLayout->addWidget(mIdListButton, 0, 1);
}

void PropertyWidget::onMouseEnteredIdWidget(const qReal::Id &id)
{
	emit mouseEnteredIdWidget(id);
}

void PropertyWidget::onMouseLeavedIdWidget(const qReal::Id &id)
{
	emit mouseLeavedIdWidget(id);
}

void PropertyWidget::onIdListButtonClicked()
{
	emit idListButtonClicked(mPropertyName);
}
