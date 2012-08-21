#include "checkoutDialog.h"
#include "../../../qrkernel/settingsManager.h"

using namespace versioning::ui;

CheckoutDialog::CheckoutDialog(QWidget *parent)
	: QDialog(parent)
{
	mBrowseButton = createButton(tr("&Browse..."), SLOT(browse()));
	mUrlLabel = new QLabel(tr("Subversion place"));
	mPathLabel = new QLabel(tr("Checkout to project:"));
	mPathComboBox = createComboBox(qReal::SettingsManager::value("checkoutTarget", "").toString());
	mUrlComboBox = createComboBox(qReal::SettingsManager::value("checkoutUrl", "").toString());
	QPushButton *ok = new QPushButton(tr("OK"), this);
	QPushButton *cancel = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mUrlLabel, 0, 0);
	mainLayout->addWidget(mUrlComboBox, 0, 1, 1, 2);
	mainLayout->addWidget(mPathLabel, 1, 0);
	mainLayout->addWidget(mPathComboBox, 1, 1);
	mainLayout->addWidget(mBrowseButton, 1, 2);
	mainLayout->addLayout(buttonsLayout,2, 0, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(tr("Checkout options"));
	setFixedSize(500,100);
}

QPushButton *CheckoutDialog::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

QComboBox *CheckoutDialog::createComboBox(const QString &text)
{
	QComboBox *comboBox = new QComboBox;
	comboBox->setEditable(true);
	comboBox->addItem(text);
	comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	return comboBox;
}

void CheckoutDialog::browse()
{
	QString saveFile = QFileDialog::getSaveFileName(this
			, tr("Select project") , QDir::currentPath()
			, tr("QReal Save File(*.qrs)"));
	if (!saveFile.isNull()) {
		mPathComboBox->addItem(saveFile);
		mPathComboBox->setCurrentIndex(mPathComboBox->findText(saveFile));
	}
}

QString CheckoutDialog::target() const
{
	return mPathComboBox->currentText();
}

QString CheckoutDialog::url() const
{
	return mUrlComboBox->currentText();
}
