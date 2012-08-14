#include <QtGui/QColorDialog>

#include "diffColorPreferencesDialog.h"
#include "ui_diffColorPreferencesDialog.h"
#include "../../../../qrkernel/settingsManager.h"

using namespace versioning::ui;

DiffColorPreferencesDialog::DiffColorPreferencesDialog(QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::DiffColorPreferencesDialog)
{
	mUi->setupUi(this);

	mAddedRemovedColor = mAddedRemovedColorBefore = SettingsManager::value("diffAddedRemovedColor"
		, QVariant(defaultAddedRemovedColor())).value<QColor>();
	mModifiedColor = mModifiedColorBefore = SettingsManager::value("diffModifiedColor"
		, QVariant(defaultModifiedColor())).value<QColor>();
	mHintColor = mHintColorBefore = SettingsManager::value("diffHintColor"
		, QVariant(defaultHintColor())).value<QColor>();

	setButtonColor(mUi->addedRemovedColorButton, mAddedRemovedColor);
	setButtonColor(mUi->modifiedColorButton, mModifiedColor);
}

DiffColorPreferencesDialog::~DiffColorPreferencesDialog()
{
	delete mUi;
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
	setButtonColor(mUi->addedRemovedColorButton, mAddedRemovedColor);
}

void DiffColorPreferencesDialog::on_modifiedColorButton_clicked()
{
	mModifiedColor = selectColor(mModifiedColor);
	setButtonColor(mUi->modifiedColorButton, mModifiedColor);
}


void DiffColorPreferencesDialog::on_hintColorButton_clicked()
{
	mHintColor = selectColor(mHintColor);
	setButtonColor(mUi->hintColorButton, mHintColor);
}
