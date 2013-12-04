#pragma once

#include <QString>
#include <QWidget>

namespace qReal {

class RecentProjectItem : public QWidget
{
	Q_OBJECT

public:
	explicit RecentProjectItem(QWidget *parent, QString const &projectName, QString const &projectFullName);
};
}
