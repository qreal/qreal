#pragma once

#include <QtWidgets/QListWidget>
#include <QtWidgets/QDialog>

#include "listWidget.h"

namespace qReal {

/// @brief ListWidget filled with recent projects
/// Associated with item data are full file path of selected recent project.
class RecentProjectsListWidget : public ListWidget
{
	Q_OBJECT

public:
	explicit RecentProjectsListWidget(QWidget *parent = 0);
};

}
