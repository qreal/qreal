#pragma once

#include <QtGui/QLabel>

#include "frame.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

int const LABEL_DEFAULT_WIDTH = 50;
int const LABEL_DEFAULT_HEIGHT = 15;

class Label : public Frame
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
	Q_PROPERTY(QString bindedPropertyName READ bindedPropertyName WRITE setBindedPropertyName USER true DESIGNABLE true)

public:
	explicit Label(ToolController *controller);

private:
	Qt::Alignment alignment() const;
	int indent() const;
	int margin() const;
	bool openExternalLinks() const;
	bool scaledContents() const;
	QString text() const;
	Qt::TextFormat textFormat() const;
	bool wordWrap() const;
	QString bindedPropertyName() const;

	void setAlignment(Qt::Alignment alignment);
	void setIndent(int indent);
	void setMargin(int margin);
	void setOpenExternalLinks(bool open);
	void setScaledContents(bool isScaled);
	void setText(QString const &text);
	void setTextFormat(Qt::TextFormat format);
	void setWordWrap(bool hasWrapping);
	void setBindedPropertyName(QString const &name);

	QLabel *mLabel;
	QString mBindedPropertyName;
};

}
}
