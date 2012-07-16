#include "versionSelector.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>

using namespace versioning::ui;

VersionSelector::VersionSelector(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Select revision"));

	QPushButton *ok = new QPushButton(tr("OK"), this);
	QPushButton *cancel = new QPushButton(tr("Cancel"), this);
	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	mRevisionSelector = new QSpinBox;
	mRevisionSelector->setMinimum(1);
	mRevisionSelector->setMaximum(2000000000);
	mRevisionSelector->setAccelerated(true);

	mHeadButton = new QRadioButton(tr("HEAD Revision"));
	mManualButton = new QRadioButton(tr("Revision"));
	mHeadButton->setChecked(true);
	connect(mHeadButton, SIGNAL(clicked()), this, SLOT(onSelectionChanged()));
	connect(mManualButton, SIGNAL(clicked()), this, SLOT(onSelectionChanged()));
	onSelectionChanged();

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mHeadButton, 0, 0);
	mainLayout->addWidget(mManualButton, 1, 0);
	mainLayout->addWidget(mRevisionSelector, 1, 1);
	mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);
	setLayout(mainLayout);
}

int VersionSelector::revision() const
{
	if (mHeadButton->isChecked()) {
		return -1;
	}
	return mRevisionSelector->value();
}

void VersionSelector::onSelectionChanged()
{
	mRevisionSelector->setEnabled(mManualButton->isChecked());
}
