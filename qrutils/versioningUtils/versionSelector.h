#pragma once

#include "../utilsDeclSpec.h"

#include <QDialog>
#include <QSpinBox>
#include <QRadioButton>

namespace versioning
{
namespace ui
{

class QRUTILS_EXPORT VersionSelector : public QDialog
{
	Q_OBJECT

public:
	explicit VersionSelector(QWidget *parent = 0);

	/// Returns version number of -1 if HEAD revision specified
	int revision() const;


private:
	QSpinBox *mRevisionSelector;
	QRadioButton *mHeadButton;
	QRadioButton *mManualButton;

private slots:
	void onSelectionChanged();
};

}
}
