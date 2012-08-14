#pragma once

#include <QtGui/QColor>
#include <QtGui/QDialog>

namespace Ui {
	class DiffColorPreferencesDialog;
}

namespace versioning
{
namespace ui
{

class DiffColorPreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit DiffColorPreferencesDialog(QWidget *parent = 0);
	~DiffColorPreferencesDialog();

	QColor addedRemovedColor() const;
	QColor modifiedColor() const;
	QColor hintColor() const;
	QColor addedRemovedColorBefore() const;
	QColor modifiedColorBefore() const;
	QColor hintColorBefore() const;
	static QColor defaultAddedRemovedColor();
	static QColor defaultModifiedColor();
	static QColor defaultHintColor();

private slots:
	void on_hintColorButton_clicked();
	void on_modifiedColorButton_clicked();
	void on_addedRemovedColorButton_clicked();

private:
	void setButtonColor(QPushButton *button, QColor const &color);
	QColor selectColor(QColor const &defaultColor);

	Ui::DiffColorPreferencesDialog *mUi;
	QColor mAddedRemovedColor;
	QColor mModifiedColor;
	QColor mAddedRemovedColorBefore;
	QColor mModifiedColorBefore;
	QColor mHintColor;
	QColor mHintColorBefore;

};

}
}
