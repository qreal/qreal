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
	/// @param shape XML Document with background description
	void setShape(QDomDocument const &shapeDocument);

	/// Returns new instance of widget described in WTF format
	/// @param document XML Document with widget template description
	static QWidget *deserializeWidget(QDomDocument const &document);

	/// Forces current editor instance to load specified widget template
	/// @param widgetTemplate XML document in WTF format
	void load(QDomDocument const &graphics);

signals:
	/// Emitted when user saves current widget template created with editor
	/// @param widget WTF format string representation
	/// @param index Metamodel elements`s template property index
	/// @param role Internal system`s role value. @see qrkernel/roles.h
	void widgetSaved(QString const &widget, QPersistentModelIndex const &index
		, int const &role);
	/// Emitted when user wants to desribe custom root backround with
	/// shape editor
	void shapeRequested(QDomDocument const &shape);
	/// Emitted when user changes his choise to create widget-based element
	/// with a choise to create shape-based one
	void changeToShapeType(QDomDocument const &shape);

protected:
	virtual void keyPressEvent(QKeyEvent *);

private slots:
	void onLayoutButtonClicked(LayoutType const type);
	void onShapeButtonClicked();
	void onSelectionChanged(Tool *tool);

	void save();
	void saveToDisk();
	void loadFromDisk();
	void preview();
	void switchToShapeType();

private:
	void initComponents();
	void initController();
	void initLayoutButtons();
	void initControlButtons();
	void initScene();
	void loadTools();
	void initPropertyBrowser();
	void initRoot();
	void initRoot(Root *root);

	void serializeWidget(QDomDocument &target);
	void preview(QWidget *widget);

	void switchLayoutButtonsActiveState(Tool *tool);

	QDomDocument shapeDocument();

	static QWidget *deserializeWidget(QDomElement const &widgetTemplate);

	Ui::WidgetsEditor *mUi;
	QGraphicsScene *mScene;
	ToolController *mController;
	LayoutButtons *mLayoutButtons;
	Root *mRoot;
	QPersistentModelIndex const mIndex;
	int const mRole;
	QDomDocument mOtherGraphics;
};

}
}
