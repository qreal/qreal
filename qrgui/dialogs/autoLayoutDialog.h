#ifndef AUTOLAYOUTDIALOG_H
#define AUTOLAYOUTDIALOG_H

#include <QDialog>
#include "../umllib/element.h"
#include "../umllib/layout/abstractLayoutHelper.h"

namespace Ui {
class AutoLayoutDialog;
}

class AutoLayoutDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AutoLayoutDialog(QWidget *parent = 0);
	~AutoLayoutDialog();

	void addLayoutHelper(AbstractLayoutHelper *);
	void setElements(QList<Element *> const &elements) { mElements = elements; }

private:
	Ui::AutoLayoutDialog *mUi;
	QList<Element *> mElements;
	QList<AbstractLayoutHelper *> mLayoutHelpers;

private slots:
	void methodSelected(QString);
	void runMethod();
};

#endif // AUTOLAYOUTDIALOG_H
