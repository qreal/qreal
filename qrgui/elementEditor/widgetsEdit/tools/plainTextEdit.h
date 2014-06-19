#pragma once

#include <QtWidgets/QPlainTextEdit>
#include <qrgui/elementEditor/widgetsEdit/private/toolController.h>

#include "abstractScrollArea.h"
#include "propertyEditor.h"

namespace qReal
{
namespace widgetsEdit
{

class PlainTextEditWidget : public QPlainTextEdit, public PropertyEditor
{
	Q_OBJECT

public:
	PlainTextEditWidget();
	virtual ~PlainTextEditWidget() {}

	virtual void setPropertyValue(QVariant const &value);

private slots:
	void onTextChanged();

private:
	bool mIgnoreNextSignal;
};

class PlainTextEditProxy : public AbstractScrollAreaProxy
{
	Q_OBJECT

	Q_PROPERTY(bool backgroundVisible READ backgroundVisible WRITE setBackgroundVisible USER true DESIGNABLE true)
	Q_PROPERTY(bool centerOnScroll READ centerOnScroll WRITE setCenterOnScroll USER true DESIGNABLE true)
	Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth USER true DESIGNABLE true)
	Q_PROPERTY(bool overwriteMode READ overwriteMode WRITE setOverwriteMode USER true DESIGNABLE true)
	Q_PROPERTY(QString plainText READ plainText WRITE setPlainText USER true DESIGNABLE true)
	Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly USER true DESIGNABLE true)
	Q_PROPERTY(bool undoRedoEnabled READ undoRedoEnabled WRITE setUndoRedoEnabled USER true DESIGNABLE true)
	Q_PROPERTY(QString binding READ propertyName WRITE setPropertyName USER true DESIGNABLE true)
	BINDING_TOOL(mPlainTextEdit)

public:
	explicit PlainTextEditProxy(PlainTextEditWidget *plainTextEdit);

private slots:
	bool backgroundVisible() const;
	bool centerOnScroll() const;
	int cursorWidth() const;
	bool overwriteMode() const;
	QString plainText() const;
	bool readOnly() const;
	bool undoRedoEnabled() const;

	void setBackgroundVisible(bool visible);
	void setCenterOnScroll(bool center);
	void setCursorWidth(int width);
	void setOverwriteMode(bool needOverwrite);
	void setPlainText(QString const &plainText);
	void setReadOnly(bool isReadOnly);
	void setUndoRedoEnabled(bool undoRedoEnabled);

	void textChanged();

private:
	PlainTextEditWidget *mPlainTextEdit;
	bool mIgnoreNextSignal;
};

class PlainTextEdit : public AbstractScrollArea
{
	Q_OBJECT

public:
	explicit PlainTextEdit(ToolController *controller);

private:
	PlainTextEditWidget *mPlainTextEdit;
};

}
}
