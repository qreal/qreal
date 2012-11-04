#pragma once
#include <QtGui/QListWidget>
#include <QtGui/QDialog>

#include "listWidget.h"

namespace qReal {

/**
 * @brief ListWidget filled with recent projects
 *
 * Associated with item data are full file path of selected recent project.
 */
class RecentProjectsListWidget : public ListWidget
{
	Q_OBJECT

public:
	explicit RecentProjectsListWidget(QDialog *parent = 0);
};

}
