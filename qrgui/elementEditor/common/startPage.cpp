#include <QtGui/QIcon>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>

#include "startPage.h"

using namespace qReal::elementEdit;

StartPage::StartPage()
{
	QIcon const shapeIcon(":/icons/widgetsEditor/shapeEditor.png");
	QIcon const widgetsIcon(":/icons/widgetsEditor/widgetEditor.png");
	QString const shapeToolTip = tr("Create shape-based element");
	QString const widgetsToolTip = tr("Create widget-based element");

	QPushButton *shapeEditorButton = new QPushButton;
	QPushButton *widgetsEditorButton = new QPushButton;
	connect(shapeEditorButton, SIGNAL(clicked())
		, this, SLOT(onShapeSelected()));
	connect(widgetsEditorButton, SIGNAL(clicked())
		, this, SLOT(onWidgetSelected()));
	shapeEditorButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	widgetsEditorButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	shapeEditorButton->setToolTip(shapeToolTip);
	widgetsEditorButton->setToolTip(widgetsToolTip);
	shapeEditorButton->setIcon(shapeIcon);
	widgetsEditorButton->setIcon(widgetsIcon);
	// TODO: Draw with non-constant size
	QSize const iconSize(300, 500);
	shapeEditorButton->setIconSize(iconSize);
	widgetsEditorButton->setIconSize(iconSize);

	QGridLayout *startLayout = new QGridLayout;
	startLayout->setMargin(3);
	startLayout->addWidget(shapeEditorButton, 0, 0);
	startLayout->addWidget(widgetsEditorButton, 0, 1);
	setLayout(startLayout);
}

void StartPage::onShapeSelected()
{
	emit shapeSelected();
}

void StartPage::onWidgetSelected()
{
	emit widgetSelected();
}
