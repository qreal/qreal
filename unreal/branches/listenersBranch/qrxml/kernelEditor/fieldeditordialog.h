#ifndef FIELDEDITORDIALOG_H
#define FIELDEDITORDIALOG_H

#include <QDialog>

namespace Ui {
    class FieldEditorDialog;
}

class FieldEditorDialog : public QDialog {
    Q_OBJECT
public:
    FieldEditorDialog(QWidget *parent = 0);
    ~FieldEditorDialog();

    QString generateFieldString();
    QString getFieldName();
    QString getFieldType();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FieldEditorDialog *ui;
};

#endif // FIELDEDITORDIALOG_H
