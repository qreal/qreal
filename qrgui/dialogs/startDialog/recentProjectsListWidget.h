#pragma once
#include <QtGui/QListWidget>
#include <QtGui/QDialog>

namespace qReal {

class RecentProjectsListWidget : public QListWidget
{
	Q_OBJECT

public:
	explicit RecentProjectsListWidget(QDialog *parent = 0);
};

}
