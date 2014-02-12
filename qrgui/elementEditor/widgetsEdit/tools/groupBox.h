#pragma once

#include <QtWidgets/QGroupBox>

#include "layoutTool.h"

namespace qReal
{
namespace widgetsEdit
{

int const GROUP_BOX_DEFAULT_WIDTH = 150;
int const GROUP_BOX_DEFAULT_HEIGHT = 150;

class GroupBoxProxy : public LayoutToolProxy
{
	Q_OBJECT

	Q_PROPERTY(Qt::Alignment alignment READ titleAlignment WRITE setTitleAlignment USER true DESIGNABLE true)
	Q_PROPERTY(bool flat READ isFlat WRITE setFlat USER true DESIGNABLE true)
	Q_PROPERTY(QString title READ title WRITE setTitle USER true DESIGNABLE true)

public:
	explicit GroupBoxProxy(QGroupBox *groupBox);

private:
	Qt::Alignment titleAlignment() const;
	bool isFlat() const;
	QString title() const;

	void setTitleAlignment(Qt::Alignment alignment);
	void setFlat(bool flat);
	void setTitle(QString const &title);

	QGroupBox *mGroupBox;

};

class GroupBox : public LayoutTool
{
	Q_OBJECT

public:
	explicit GroupBox(ToolController *controller);

private:
	QGroupBox *mGroupBox;
};

}
}
