#pragma once

#include <QtGui/QListWidget>
#include "../kernel/ids.h"
#include "error.h"

namespace qReal {
  class MainWindow;
}

class ErrorListWidget : public QListWidget
{
	Q_OBJECT

private slots:
	void clickList();

public:
	explicit ErrorListWidget(QWidget *parent = NULL);
	void init(qReal::MainWindow* mainWindow);
	void highlightElement(QListWidgetItem* const item);

private:
	qReal::MainWindow* mMainWindow;

};

