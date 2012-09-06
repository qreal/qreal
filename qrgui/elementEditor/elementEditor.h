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
	ElementEditor(QPersistentModelIndex const &index
		, int const &role, QWidget *parent = 0);
	ElementEditor(QWidget *parent = 0);

	ShapeEdit *shapeEditor() const;
	widgetsEdit::WidgetsEditor *widgetEditor() const;
	bool widgetBased() const;
	void load(QString const &data);

signals:
	void widgetSaved(QString const &widget, QPersistentModelIndex const &index
			, int const &role);
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
	void initComponents();
	void initStartWidget();

	void showShapeEditor();
	void showWidgetsEditor();
	void showWidget(QWidget *widget);

	void giveShapeToWidgetsEditor(QDomDocument const &document);
	QDomDocument makeSdfShape(QDomDocument const &document);

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
