#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsProxyWidget>
#include <QtCore/QPersistentModelIndex>

#include "tools/tool.h"
#include "tools/root.h"
#include "tools/layoutHelpers/layoutTypes.h"
#include "private/toolController.h"
#include "private/layoutButtons.h"
#include "../../../qrutils/navigationUtils/navigationPageWithMenu.h"
#include "../../umllib/sdfRenderer.h"
#include "../../editorPluginInterface/propertyEditorInterface.h"
#include "../common/controlButtons.h"
#include "../common/templateDocumentBuilder.h"

namespace Ui
{
	class WidgetsEditor;
}

namespace qReal
{
namespace widgetsEdit
{

class WidgetsEditor : public QWidget, public navigation::NavigationPageWithMenu
{
	Q_OBJECT

public:
	WidgetsEditor(bool isIconEditor, QWidget *parent = 0);
	~WidgetsEditor();

	/// Forces widget template root element to have
	/// custom backround described with SDF format
	/// @param shape XML Document with background description
//	void setShape(QDomDocument const &shapeDocument);

	/// Returns new instance of widget described in WTF format
	/// @param document XML Document with widget template description
	/// @param editors All property editors will be added here
	static QWidget *deserializeWidget(QDomDocument const &document
			, QList<PropertyEditorInterface *> &editors);

	/// Forces current editor instance to load specified widget template
	/// @param widgetTemplate XML document in WTF format
	void load(QDomDocument const &graphics);

	/// Returns WTF document with current model (even unsaved)
	QDomDocument currentTemplate();

	qReal::elementEdit::ControlButtons *controlButtons() const;

signals:
	/// Emitted when user saves current widget template created with editor
	/// @param widget WTF format string representation
	void widgetSaved();

protected:
	virtual void keyPressEvent(QKeyEvent *);

	virtual void onShown(navigation::NavigationState *state);

private slots:
	void onLayoutButtonClicked(LayoutType const type);
	void onSelectionChanged(Tool *tool);

	void save();
	void saveToDisk();
	void loadFromDisk();
	void preview();
	void editOuterBindings();

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
	void initEmptyCase();

	void serializeWidget(QDomDocument &target);
	void preview(QWidget *widget);

	void switchLayoutButtonsActiveState(Tool *tool);

	static QWidget *deserializeWidget(QDomElement const &widgetTemplate
			, QList<PropertyEditorInterface *> &editors);

	bool mIsIconEditor;
	Ui::WidgetsEditor *mUi;
	QGraphicsScene *mScene;
	ToolController *mController;
	LayoutButtons *mLayoutButtons;
	qReal::elementEdit::ControlButtons *mControlButtons;
	Root *mRoot;
	qReal::elementEdit::TemplateDocumentBuilder *mDocumentBuilder;
	QDomDocument mEmptyCaseWtf;
	QtTreePropertyBrowser *mProbertyBrowser;
	PropertyBrowserController *mPropertyBrowserController;
};

}
}
