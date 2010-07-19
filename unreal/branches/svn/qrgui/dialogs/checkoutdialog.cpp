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

//	QHBoxLayout *buttonsLayout = new QHBoxLayout;
//	buttonsLayout->addStretch();

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(urlLabel,0,0);
	mainLayout->addWidget(urlComboBox, 0, 1, 1, 2);
	mainLayout->addWidget(directoryLabel, 1, 0);
	mainLayout->addWidget(directoryComboBox, 1, 1);
	mainLayout->addWidget(browseButton, 1, 2);
//	mainLayout->addLayout(buttonsLayout);
	setLayout(mainLayout);

	setWindowTitle(tr("Checkout options"));
	resize(200,200);
}
void CheckoutDialog::browse()
{
	QString directory = QFileDialog::getExistingDirectory(this,
							   tr("Select directory"), QDir::currentPath());

	//if (directory.isEmpty()) {
	//	if (directoryComboBox->findText(directory) == -1)
	if (!directory.isNull()) {
			directoryComboBox->addItem(directory);
			directoryComboBox->setEditText(directory);
	//	directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
	//}
		}
}
