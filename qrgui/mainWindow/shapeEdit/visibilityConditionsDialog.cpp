#include "visibilityConditionsDialog.h"
#include "ui_visibilityConditionsDialog.h"

#include <QtWidgets/QPushButton>

VisibilityConditionsDialog::VisibilityConditionsDialog(QMap<QString, PropertyInfo> const &properties
		, QList<Item *> const &items, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::VisibilityConditionsDialog)
	, mProperties(properties), mItems(items)
{
	ui->setupUi(this);

	ui->propertyComboBox->addItem(QString());
	ui->propertyComboBox->addItems(properties.keys());

	setWidgetValues();

	connect(ui->propertyComboBox, SIGNAL(activated(QString const &)), this, SLOT(changeProperty(QString const &)));
	connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(okClicked()));
}

VisibilityConditionsDialog::~VisibilityConditionsDialog()
{
	delete ui;
}

void VisibilityConditionsDialog::changeProperty(QString const &propertyName)
{
	PropertyInfo propertyInfo = mProperties[propertyName];
	ui->valueWidget->setPropertyInfo(propertyInfo);
	changeOperators(propertyInfo.type);
}

void VisibilityConditionsDialog::changeOperators(Type type)
{
	QStringList operators;
	operators << "=" << "!=";

	if (type == Int) {
		operators << ">" << "<" << ">=" << "<=";
	} else if (type == String) {
		operators << "=~";
	}

	ui->operatorComboBox->clear();
	ui->operatorComboBox->addItems(operators);
}

void VisibilityConditionsDialog::okClicked()
{
	foreach (Item *item, mItems) {
		item->setVisibilityCondition(ui->propertyComboBox->currentText()
				, ui->operatorComboBox->currentText(), ui->valueWidget->value());
	}
}

void VisibilityConditionsDialog::setWidgetValues()
{
	if (areValuesEqual()) {
		Item::VisibilityCondition condition = mItems.first()->visibilityCondition();

		ui->propertyComboBox->setCurrentIndex(ui->propertyComboBox->findText(condition.property));
		changeProperty(ui->propertyComboBox->currentText());
		ui->operatorComboBox->setCurrentIndex(ui->operatorComboBox->findText(condition.sign));
		ui->valueWidget->setValue(condition.value);
	}
}

bool VisibilityConditionsDialog::areValuesEqual() const
{
	Item::VisibilityCondition value = mItems.first()->visibilityCondition();
	foreach (Item *item, mItems) {
		if (item->visibilityCondition() != value) {
			return false;
		}
	}
	return true;
}

VisibilityConditionsDialog::PropertyInfo::PropertyInfo(VisibilityConditionsDialog::Type t, QStringList const &v)
		: type(t), values(v)
{}
