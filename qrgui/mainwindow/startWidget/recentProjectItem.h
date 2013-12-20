#pragma once

#include <QtWidgets/QPushButton>

namespace qReal {

/// Represents a button in recent projects list on the start tab
class RecentProjectItem : public QPushButton
{
	Q_OBJECT

public:
	RecentProjectItem(QWidget *parent, QString const &projectName, QString const &projectFullName);
};

}
