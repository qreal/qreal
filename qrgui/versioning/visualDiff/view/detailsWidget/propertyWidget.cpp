#include "propertyWidget.h"
#include "../diffColorPreferencesDialog.h"
#include "../../model/diffState.h"

using namespace versioning;
using namespace versioning::details;

PropertyWidget::PropertyWidget(DiffModel *diffModel
		, bool isGraphical, QWidget *parent)
	: QWidget(parent), mDiffModel(diffModel), mIsGraphical(isGraphical)
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

	DiffState const state = difference()->propertyState(propertyName);
	switch(state) {
	case Added:
		stretchLayout(1);
		setPropertyValue(mNewValueLabel, mNewIdWidget, difference()->newProperty(mPropertyName), 1);
		break;
	case Removed:
	case Same:
		stretchLayout(1);
		setPropertyValue(mOldValueLabel, mOldIdWidget, difference()->oldProperty(mPropertyName), 1);
		break;
	case Modified:
		QColor const color = SettingsManager::value("diffModifiedColor"
			, ui::DiffColorPreferencesDialog::defaultModifiedColor()).value<QColor>();
		QPalette palette = mPropertyNameLabel->palette();
		palette.setColor(QPalette::Foreground, color);
		mPropertyNameLabel->setPalette(palette);
		stretchLayout(2);
		setPropertyValue(mOldValueLabel, mOldIdWidget, difference()->oldProperty(mPropertyName), 1);
		setPropertyValue(mNewValueLabel, mNewIdWidget, difference()->newProperty(mPropertyName), 2);
		break;
	}
}

void PropertyWidget::reset()
{
	mPropertyNameLabel->setText("");
}

CommonDifference *PropertyWidget::difference() const
{
	return mIsGraphical
		? static_cast<CommonDifference *>(mDiffModel->difference(mGraphicalId)->graphicalDifference())
		: static_cast<CommonDifference *>(mDiffModel->difference(mLogicalId)->logicalDifference());
}

qReal::Id PropertyWidget::id() const
{
	return mIsGraphical? mGraphicalId : mLogicalId;
}

void PropertyWidget::stretchLayout(int columnCount)
{
	mLayout->setRowStretch(0, 10);
	if (1 == columnCount) {
		mLayout->setRowStretch(1, 40);
	} else {
		mLayout->setRowStretch(1, 20);
		mLayout->setRowStretch(2, 20);
	}
}

void PropertyWidget::setPropertyValue(QLabel *label, IdWidget *idWidget, QVariant value, int column)
{
	if (value.type() == QVariant::UserType) {
		if (value.userType() == QMetaType::type("qReal::Id")) {
			createIdWidget(idWidget, value, column);
			return;
		}
		if (value.userType() == QMetaType::type("qReal::IdList")) {
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
	qReal::Id const id = value.value<qReal::Id>();
	if (mIsGraphical) {
		idWidget->setId(id, qReal::Id());
	} else {
		idWidget->setId(qReal::Id(), id);
	}
	connect(idWidget, SIGNAL(mouseEntered(qReal::Id))
			, this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(idWidget, SIGNAL(mouseLeaved(qReal::Id))
			, this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	mLayout->addWidget(idWidget, 0, column);
}

void PropertyWidget::createButton()
{
	mIdListButton = new QPushButton(this);
	mIdListButton->setText(tr("View id list difference"));
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
