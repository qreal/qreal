#pragma once
#include <QtWidgets/QDialog>

namespace qReal {

/// Dialog with managed closability
class ManagedClosableDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ManagedClosableDialog(QWidget *parent = 0, bool isClosable = true);
	void closeEvent(QCloseEvent *event);
	void keyPressEvent(QKeyEvent *event);

public slots:
	void setClosability(bool isClosable);
	bool forceClose();

private:
	bool mIsClosable;
};

}
