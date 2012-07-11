#pragma once
#include <QtGui/QListWidget>
#include <QtGui/QDialog>
#include "listWidget.h"

namespace qReal {

class RecentProjectsListWidget : public ListWidget
{
	Q_OBJECT

public:
	explicit RecentProjectsListWidget(QDialog *parent = 0);
};

}
