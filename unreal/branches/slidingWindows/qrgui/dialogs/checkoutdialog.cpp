#include "checkoutdialog.h"

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

CheckoutDialog::CheckoutDialog(QWidget *parent) : QDialog(parent)
{
	browseButton = createButton(tr("&Browse..."), SLOT(browse()));
	urlLabel = new QLabel(tr("Subversion place"));
	directoryLabel = new QLabel(tr("Checkout to directory:"));
	directoryComboBox = createComboBox(QDir::currentPath());
	urlComboBox = createComboBox();
	QPushButton *ok = new QPushButton(tr("Ok"), this);
	QPushButton *cancel = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(urlLabel,0,0);
	mainLayout->addWidget(urlComboBox, 0, 1, 1, 2);
	mainLayout->addWidget(directoryLabel, 1, 0);
	mainLayout->addWidget(directoryComboBox, 1, 1);
	mainLayout->addWidget(browseButton, 1, 2);
	mainLayout->addLayout(buttonsLayout,2, 0, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(tr("Checkout options"));
	setFixedSize(500,100);
}
void CheckoutDialog::browse()
{
	QString directory = QFileDialog::getExistingDirectory(this,
						tr("Select directory"), QDir::currentPath());
	if (!directory.isNull()) {
		directoryComboBox->addItem(directory);
		directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
	}
}
QString CheckoutDialog::getDir()
{
	return directoryComboBox->currentText();
}
QString CheckoutDialog::getUrl()
{
	return urlComboBox->currentText();
}

