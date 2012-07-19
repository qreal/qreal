#pragma once

#include <QDialog>

#include "../utilsDeclSpec.h"

namespace Ui {
class VersionSelector;
}

namespace versioning
{
namespace ui
{

/// @brief Dialog providing posibility to select revision.
class QRUTILS_EXPORT VersionSelector : public QDialog
{
	Q_OBJECT

public:
	explicit VersionSelector(QWidget *parent = 0);
	~VersionSelector();

	/// Returns version number of -1 if HEAD revision specified
	int revision() const;

private slots:
	void onSelectionChanged();

private:
	Ui::VersionSelector *mUi;
};

}
}
