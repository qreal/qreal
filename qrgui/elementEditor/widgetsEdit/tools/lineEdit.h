#pragma once

#include <QtWidgets/QLineEdit>

#include "tool.h"
#include "../private/toolController.h"
#include "propertyEditor.h"

namespace qReal
{
namespace widgetsEdit
{

class LineEditWidget : public QLineEdit, public PropertyEditor
{
	Q_OBJECT

public:
	LineEditWidget();
	virtual ~LineEditWidget() {}

	virtual void setPropertyValue(QVariant const &value);

private slots:
	void onTextChanged(QString const &text);
};

class LineEditProxy : public ToolProxy
{
	Q_OBJECT

	Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment USER true DESIGNABLE true)
	Q_PROPERTY(QLineEdit::EchoMode echoMode READ echoMode WRITE setEchoMode USER true DESIGNABLE true)
	Q_PROPERTY(bool frame READ frame WRITE setFrame USER true DESIGNABLE true)
	Q_PROPERTY(QString inputMask READ inputMask WRITE setInputMask USER true DESIGNABLE true)
	Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength USER true DESIGNABLE true)
	Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText USER true DESIGNABLE true)
	Q_PROPERTY(bool readonly READ readonly WRITE setReadonly USER true DESIGNABLE true)
	Q_PROPERTY(QString text READ text WRITE setText USER true DESIGNABLE true)
	Q_PROPERTY(QString binding READ propertyName WRITE setPropertyName USER true DESIGNABLE true)
	BINDING_TOOL(mLineEdit)

public:
	explicit LineEditProxy(LineEditWidget *lineEdit);

private slots:
	void textChanged(QString const &text);

private:
	Qt::Alignment alignment() const;
	QLineEdit::EchoMode echoMode() const;
	bool frame() const;
	QString inputMask() const;
	int maxLength() const;
	QString placeholderText() const;
	bool readonly() const;
	QString text() const;

	void setAlignment(Qt::Alignment alignment);
	void setEchoMode(QLineEdit::EchoMode echoMode);
	void setFrame(bool isFrame);
	void setInputMask(QString const &inputMask);
	void setMaxLength(int maxLength);
	void setPlaceholderText(QString const &placeholderText);
	void setReadonly(bool isReadonly);
	void setText(QString const &text);

	LineEditWidget *mLineEdit;
};

class LineEdit : public Tool
{
	Q_OBJECT

public:
	explicit LineEdit(ToolController *controller);

	LineEditWidget *mLineEdit;
};

}
}
