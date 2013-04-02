#include <QPushButton>

#include "visibilityConditionsDialog.h"
#include "ui_visibilityConditionsDialog.h"

VisibilityConditionsDialog::VisibilityConditionsDialog(QMap<QString, QStringList> const &enumValues
		, QList<Item *> const &items, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::VisibilityConditionsDialog)
	, mEnumValues(enumValues), mItems(items)
{
	ui->setupUi(this);

	ui->propertyComboBox->addItem(QString());
	ui->propertyComboBox->addItems(enumValues.keys());

	setIndices();

	connect(ui->propertyComboBox, SIGNAL(activated(QString const &)), this, SLOT(changeValues(QString const &)));
	connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(okClicked()));
}

VisibilityConditionsDialog::~VisibilityConditionsDialog()
{
	delete ui;
}

void VisibilityConditionsDialog::changeValues(QString const &enumName)
{
	ui->valueComboBox->clear();
	ui->valueComboBox->addItems(mEnumValues[enumName]);
}

void VisibilityConditionsDialog::okClicked()
{
	foreach (Item *item, mItems) {
		item->setVisibilityCondition(ui->propertyComboBox->currentText(), ui->valueComboBox->currentText());
	}
}

void VisibilityConditionsDialog::setIndices()
{
	if (areValuesEqual()) {
		ui->propertyComboBox->setCurrentIndex(
					ui->propertyComboBox->findText(mItems.first()->visibilityCondition().property));
		ui->valueComboBox->addItems(mEnumValues[ui->propertyComboBox->currentText()]);
		ui->valueComboBox->setCurrentIndex(ui->valueComboBox->findText(mItems.first()->visibilityCondition().value));
	}
}

bool VisibilityConditionsDialog::areValuesEqual()
{
	Item::VisibilityCondition value = mItems.first()->visibilityCondition();
	foreach (Item *item, mItems) {
		if (item->visibilityCondition().property != value.property
				|| item->visibilityCondition().value != value.value) {
			return false;
		}
	}
	return true;
}
