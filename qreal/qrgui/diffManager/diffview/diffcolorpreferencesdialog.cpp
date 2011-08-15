#include "diffcolorpreferencesdialog.h"
#include "ui_diffcolorpreferencesdialog.h"

#include <QSettings>
#include <QColorDialog>

DiffColorPreferencesDialog::DiffColorPreferencesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DiffColorPreferencesDialog)
{
	ui->setupUi(this);

	QSettings settings("SPbSU", "QReal");

	mAddedRemovedColor = mAddedRemovedColorBefore = settings.value("diffAddedRemovedColor",
					QVariant(defaultAddedRemovedColor())).value<QColor>();
	mModifiedColor = mModifiedColorBefore = settings.value("diffModifiedColor",
					QVariant(defaultModifiedColor())).value<QColor>();
	mHintColor = mHintColorBefore = settings.value("diffHintColor",
					QVariant(defaultHintColor())).value<QColor>();

	setButtonColor(ui->addedRemovedColorButton, mAddedRemovedColor);
	setButtonColor(ui->modifiedColorButton, mModifiedColor);
}

DiffColorPreferencesDialog::~DiffColorPreferencesDialog()
{
	delete ui;
}

QColor DiffColorPreferencesDialog::addedRemovedColor() const
{
	return mAddedRemovedColor;
}

QColor DiffColorPreferencesDialog::modifiedColor() const
{
	return mModifiedColor;
}

QColor DiffColorPreferencesDialog::hintColor() const
{
	return mHintColor;
}

QColor DiffColorPreferencesDialog::addedRemovedColorBefore() const
{
	return mAddedRemovedColorBefore;
}

QColor DiffColorPreferencesDialog::modifiedColorBefore() const
{
	return mModifiedColorBefore;
}

QColor DiffColorPreferencesDialog::hintColorBefore() const
{
	return mHintColorBefore;
}

QColor DiffColorPreferencesDialog::defaultAddedRemovedColor()
{
	return QColor(Qt::green);
}

QColor DiffColorPreferencesDialog::defaultModifiedColor()
{
	return QColor(255, 165, 0);
}

QColor DiffColorPreferencesDialog::defaultHintColor()
{
	return QColor(0, 0, 255);
}

void DiffColorPreferencesDialog::setButtonColor(QPushButton *button, const QColor &color)
{
	QPalette p = button->palette();
	p.setColor(QPalette::Button, color);
	button->setAutoFillBackground(true);
	button->setPalette(p);
}

QColor DiffColorPreferencesDialog::selectColor(const QColor &defaultColor)
{
	QColorDialog dialog(defaultColor, this);
	if (QDialog::Accepted == dialog.exec())
	{
		return dialog.currentColor();
	}
	return defaultColor;
}

void DiffColorPreferencesDialog::on_addedRemovedColorButton_clicked()
{
	mAddedRemovedColor = selectColor(mAddedRemovedColor);
	setButtonColor(ui->addedRemovedColorButton, mAddedRemovedColor);
}

void DiffColorPreferencesDialog::on_modifiedColorButton_clicked()
{
	mModifiedColor = selectColor(mModifiedColor);
	setButtonColor(ui->modifiedColorButton, mModifiedColor);
}


void DiffColorPreferencesDialog::on_hintColorButton_clicked()
{
	mHintColor = selectColor(mHintColor);
	setButtonColor(ui->hintColorButton, mHintColor);
}
