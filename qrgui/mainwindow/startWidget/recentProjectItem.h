#pragma once

#include <QtWidgets/QPushButton>

namespace qReal {

class RecentProjectItem : public QPushButton
{
	Q_OBJECT

public:
	explicit RecentProjectItem(QWidget *parent, QString const &projectName, QString const &projectFullName);
};

}
