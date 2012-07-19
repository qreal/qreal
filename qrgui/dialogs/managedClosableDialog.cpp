#include <QtGui/QtEvents>

#include "managedClosableDialog.h"

using namespace qReal;

ManagedClosableDialog::ManagedClosableDialog(QWidget *parent, bool isClosable)
        : QDialog(parent, isClosable ? Qt::Dialog : Qt::WindowMinimizeButtonHint)
        , mIsClosable(isClosable)
{
}

void ManagedClosableDialog::setClosability(bool isClosable)
{
    mIsClosable = isClosable;
}

bool ManagedClosableDialog::forceClose()
{
    setClosability(true);
    return close();
}

void ManagedClosableDialog::closeEvent(QCloseEvent *event)
{
    event->setAccepted(mIsClosable);
}

void ManagedClosableDialog::keyPressEvent(QKeyEvent *event)
{
    if (!mIsClosable && event->key() == Qt::Key_Escape) {
        event->ignore();
        return;
    }
    QDialog::keyPressEvent(event);
}
