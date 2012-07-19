#pragma once
#include <QDialog>

namespace qReal {

/**
 * @brief Dialog with managed closability
 */
class ManagedClosableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManagedClosableDialog(QWidget *parent = 0, bool isClosable = true);
    bool forceClose();
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

public slots:
    void setClosability(bool isClosable);

private:
    bool mIsClosable;
};

}
