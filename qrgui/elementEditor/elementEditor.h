#pragma once

#include <QtGui/QWidget>
#include <QtGui/QStackedLayout>

#include "shapeEdit/shapeEdit.h"
#include "widgetsEdit/widgetsEditor.h"

namespace qReal
{

class ElementEditor : public QWidget
{
	Q_OBJECT

public:
	/// Creates new element editior instance binded to specified model item
	/// @param index Metamodel elements`s template property index
	/// @param role Internal system`s role value. @see qrkernel/roles.h
	ElementEditor(QPersistentModelIndex const &index
		, int const &role, QWidget *parent = 0);
	/// Creates new element editor instance binded to nothing
	ElementEditor(QWidget *parent = 0);

	/// Returns shape editor instance
	ShapeEdit *shapeEditor() const;
	/// Returns widgets editor instance
	widgetsEdit::WidgetsEditor *widgetEditor() const;
	bool isWidgetBased() const;
	/// Forces current editor to load specified data
	/// @param data String representation of data saved by required editor in past
	void load(QString const &data);

signals:
	/// Emitted when user saves current widget template created with widget editor
	/// @param widget WTF format string representation
	/// @param index Metamodel elements`s template property index
	/// @param role Internal system`s role value. @see qrkernel/roles.h
	void widgetSaved(QString const &widget, QPersistentModelIndex const &index
			, int const &role);
	/// Emitted when user saves current shape created with shape editor
	/// @param shape SDF format string representation
	/// @param index Metamodel elements`s template property index
	/// @param role Internal system`s role value. @see qrkernel/roles.h
	void shapeSaved(QString const &shape, QPersistentModelIndex const &index
			, int const &role);

private slots:
	void onWidgetBasedButtonClicked();
	void onShapeBasedButtonClicked();
	void onWidgetEditorRequestedShape();
	void onWidgetEditorSavedShape(QString const &widget
		, QPersistentModelIndex const &index, int const &role);
	void onShapeEditorSavedShape(QString const &shape
		, QPersistentModelIndex const &index, int const &role);
	void onShapeEditorSavedShape(QDomDocument const &document);

private:
	static bool isWidgetBasedDocument(QDomDocument const &document);

	void initComponents();
	void initStartWidget();

	void showShapeEditor();
	void showWidgetsEditor();
	void showWidget(QWidget *widget);

	void giveShapeToWidgetsEditor(QDomDocument const &document);

	QPersistentModelIndex mIndex;
	int mRole;
	bool mWidgetBased;
	QStackedLayout *mLayout;
	QWidget *mStartWidget;
	widgetsEdit::WidgetsEditor *mWidgetsEditor;
	ShapeEdit *mShapeEditor;
	bool mOpenedFromMetaEditor;

};

}
