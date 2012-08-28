#pragma once

#include <QtGui/QWidget>
#include <QtGui/QGraphicsProxyWidget>
#include <QtCore/QPersistentModelIndex>

#include "tools/tool.h"
#include "tools/root.h"
#include "tools/layoutHelpers/layoutTypes.h"
#include "private/toolController.h"
#include "private/layoutButtons.h"
#include "../../../umllib/sdfRenderer.h"

namespace Ui
{
	class WidgetsEditor;
}

using namespace Ui::WidgetsEdit;

class WidgetsEditor : public QWidget
{
	Q_OBJECT

public:
	WidgetsEditor(QPersistentModelIndex const &index
		, int const &role, QWidget *parent = 0);
	WidgetsEditor(QWidget *parent = 0);
	~WidgetsEditor();

	void setShape(QDomDocument const &shape);

	static QWidget *deserializeWidget(QDomDocument const &document);

signals:
	void widgetSaved(QString const &widget, QPersistentModelIndex const &index
		, int const &role);
	void shapeRequested();

protected:
	virtual void keyPressEvent(QKeyEvent *);

private slots:
	void onLayoutButtonClicked(LayoutType type);
	void onShapeButtonClicked();
	void onSelectionChanged(Tool *tool);

	void save();

private:
	void initComponents();
	void initController();
	void initLayoutButtons();
	void initShapeButton();
	void initSaveButton();
	void initScene();
	void loadTools();
	void initPropertyBrowser();
	void initRoot();

	void switchLayoutButtonsActiveState(Tool *tool);

	Ui::WidgetsEditor *mUi;
	QGraphicsScene *mScene;
	ToolController *mController;
	LayoutButtons *mLayoutButtons;
	QPushButton *mShapeButton;
	Ui::WidgetsEdit::Root *mRoot;
	QPersistentModelIndex const mIndex;
	int const mRole;

};
