#pragma once

#include <QtGui/QPlainTextEdit>

#include "abstractScrollArea.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class PlainTextEdit : public AbstractScrollArea
{
	Q_OBJECT

	Q_PROPERTY(bool backgroundVisible READ backgroundVisible WRITE setBackgroundVisible USER true DESIGNABLE true)
	Q_PROPERTY(bool centerOnScroll READ centerOnScroll WRITE setCenterOnScroll USER true DESIGNABLE true)
	Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth USER true DESIGNABLE true)
	Q_PROPERTY(bool overwriteMode READ overwriteMode WRITE setOverwriteMode USER true DESIGNABLE true)
	Q_PROPERTY(QString plainText READ plainText WRITE setPlainText USER true DESIGNABLE true)
	Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly USER true DESIGNABLE true)
	Q_PROPERTY(bool undoRedoEnabled READ undoRedoEnabled WRITE setUndoRedoEnabled USER true DESIGNABLE true)

public:
	explicit PlainTextEdit(ToolController *controller);

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
	QPlainTextEdit *mPlainTextEdit;

};

}
}
