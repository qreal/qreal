#pragma once

#include <QtGui/QWidget>

#include "../../qrutils/navigationUtils/navigationView.h"
#include "common/templateDocumentBuilder.h"
#include "common/startPage.h"
#include "shapeEdit/shapeEdit.h"
#include "widgetsEdit/widgetsEditor.h"

namespace qReal
{

class ElementEditor : public navigation::NavigationView
{
	Q_OBJECT

public:
	/// Creates new element editior instance binded to specified model item
	/// @param index Metamodel elements`s template property index
	/// @param role Internal system`s role value. @see qrkernel/roles.h
	ElementEditor(QPersistentModelIndex const &index
		, int const &role, QWidget *parent = 0);
	virtual ~ElementEditor();

	/// Returns shape editor instance
	ShapeEdit *shapeEditor() const;
	/// Returns widgets editor instance
	widgetsEdit::WidgetsEditor *widgetEditor() const;
	/// Forces current editor to load specified data
	/// @param data String representation of data saved by required editor in past
	void load(QString const &data);
	/// Forces current editor to load specified data
	/// @param data Data saved by required editor in past
	void load(QDomDocument const &data);

signals:
	/// Emitted when user saves current widget template created with widget editor
	/// @param widget WTF document string representation
	/// @param index Metamodel elements`s template property index
	/// @param role Internal system`s role value. @see qrkernel/roles.h
	void widgetSaved(QString const &widget, QPersistentModelIndex const &index
			, int const &role);
	/// Emitted when user saves current shape created with shape editor
	/// @param shape SDF document string respesentation
	/// @param index Metamodel elements`s template property index
	/// @param role Internal system`s role value. @see qrkernel/roles.h
	void shapeSaved(QString const &shape, QPersistentModelIndex const &index
			, int const &role);

private slots:
	void switchToRegularWidgetsEditor();
	void switchToRegularShapeEditor();
	void switchToIconWidgetsEditor();
	void switchToIconShapeEditor();
	void switchToIconEditor();
	void switchToRegularEditor();
	void switchToWidgetsEditor();
	void switchToShapeEditor();

	void onWidgetSaved();
	void onShapeSaved();

private:
	void initRegularWidgetsEditor();
	void initRegularShapeEditor();
	void initIconWidgetsEditor();
	void initIconShapeEditor();

	void initStartPageInteraction(qReal::elementEdit::StartPage *startPage);
	void initControlButtonsInteraction(qReal::elementEdit::ControlButtons *buttons);

	bool isEditingIcon() const;

	QPersistentModelIndex mIndex;
	int mRole;
	widgetsEdit::WidgetsEditor *mWidgetsEditor;
	ShapeEdit *mShapeEditor;
	widgetsEdit::WidgetsEditor *mIconWidgetsEditor;
	ShapeEdit *mIconShapeEditor;
	elementEdit::TemplateDocumentBuilder *mDocumentBuilder;
};

}
