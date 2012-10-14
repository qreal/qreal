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

namespace qReal
{
namespace widgetsEdit
{

class WidgetsEditor : public QWidget
{
	Q_OBJECT

public:
	WidgetsEditor(QPersistentModelIndex const &index
		, int const &role, QWidget *parent = 0);
	explicit WidgetsEditor(QWidget *parent = 0);
	~WidgetsEditor();

	/// Forces widget template root element to have
	/// custom backround described with SDF format
	/// @param shape XML Doucment with background description
	void setShape(QDomDocument const &shape);

	/// Returns new instance of widget described in WTF format
	/// @param document XML Doucment with widget template description
	static QWidget *deserializeWidget(QDomDocument const &document);

signals:
	/// Emitted when user saves current widget template created with editor
	/// @param widget WTF format string representation
	/// @param index Metamodel elements`s template property index
	/// @param role Internal system`s role value. @see qrkernel/roles.h
	void widgetSaved(QString const &widget, QPersistentModelIndex const &index
		, int const &role);
	/// Emitted when user wants to desribe custom root backround with
	/// shape editor
	void shapeRequested();

protected:
	virtual void keyPressEvent(QKeyEvent *);

private slots:
	void onLayoutButtonClicked(LayoutType const type);
	void onShapeButtonClicked();
	void onSelectionChanged(Tool *tool);

	void save();
	void preview();

private:
	void initComponents();
	void initController();
	void initLayoutButtons();
	void initShapeButton();
	void initSaveButton();
	void initPreviewButton();
	void initScene();
	void loadTools();
	void initPropertyBrowser();
	void initRoot();

	void serializeWidget(QDomDocument &target);

	void preview(QWidget *widget);

	void switchLayoutButtonsActiveState(Tool *tool);

	Ui::WidgetsEditor *mUi;
	QGraphicsScene *mScene;
	ToolController *mController;
	LayoutButtons *mLayoutButtons;
	QPushButton *mShapeButton;
	Root *mRoot;
	QPersistentModelIndex const mIndex;
	int const mRole;

};

}
}
