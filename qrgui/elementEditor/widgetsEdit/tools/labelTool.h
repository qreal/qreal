#pragma once

#include <QtWidgets/QLabel>

#include "frame.h"
#include "../private/toolController.h"
#include "propertyEditor.h"

namespace qReal
{
namespace widgetsEdit
{

int const LABEL_DEFAULT_WIDTH = 50;
int const LABEL_DEFAULT_HEIGHT = 15;

class LabelWidget : public QLabel, public PropertyEditor
{
	Q_OBJECT

public:
	LabelWidget(QString const &text);
	virtual ~LabelWidget() {}

	virtual void setPropertyValue(QVariant const &value);
};

class LabelProxy : public FrameProxy
{
	Q_OBJECT

	Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment USER true DESIGNABLE true)
	Q_PROPERTY(int indent READ indent WRITE setIndent USER true DESIGNABLE true)
	Q_PROPERTY(int margin READ margin WRITE setMargin USER true DESIGNABLE true)
	Q_PROPERTY(bool openExternalLinks READ openExternalLinks WRITE setOpenExternalLinks USER true DESIGNABLE true)
	Q_PROPERTY(bool scaledContents READ scaledContents WRITE setScaledContents USER true DESIGNABLE true)
	Q_PROPERTY(QString text READ text WRITE setText USER true DESIGNABLE true)
	Q_PROPERTY(Qt::TextFormat textFormat READ textFormat WRITE setTextFormat USER true DESIGNABLE true)
	Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap USER true DESIGNABLE true)
	Q_PROPERTY(QString binding READ propertyName WRITE setPropertyName USER true DESIGNABLE true)
	BINDING_TOOL(mLabel)

public:
	explicit LabelProxy(LabelWidget *label);

private:
	Qt::Alignment alignment() const;
	int indent() const;
	int margin() const;
	bool openExternalLinks() const;
	bool scaledContents() const;
	QString text() const;
	Qt::TextFormat textFormat() const;
	bool wordWrap() const;

	void setAlignment(Qt::Alignment alignment);
	void setIndent(int indent);
	void setMargin(int margin);
	void setOpenExternalLinks(bool open);
	void setScaledContents(bool isScaled);
	void setText(QString const &text);
	void setTextFormat(Qt::TextFormat format);
	void setWordWrap(bool hasWrapping);

	LabelWidget *mLabel;
};

class LabelTool : public Frame
{
	Q_OBJECT

public:
	explicit LabelTool(ToolController *controller);

private:
	LabelWidget *mLabel;
};

}
}
