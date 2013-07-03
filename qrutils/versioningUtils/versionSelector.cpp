#include "versionSelector.h"
#include "ui_versionSelector.h"

using namespace versioning::ui;

VersionSelector::VersionSelector(QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::VersionSelector)
{
	mUi->setupUi(this);
	onSelectionChanged();
}

VersionSelector::~VersionSelector()
{
	delete mUi;
}

int VersionSelector::revision() const
{
	if (mUi->headButton->isChecked()) {
		return -1;
	}
	return mUi->revisionSelector->value();
}

void VersionSelector::onSelectionChanged()
{
	mUi->revisionSelector->setEnabled(mUi->manualButton->isChecked());
}
