#pragma once

#include <QtWidgets/QWidget>
#include <QtXml/QDomDocument>
#include <qrgui/editorPluginInterface/elementRepoInterface.h>

#include "../umllib/sdfRenderer.h"

namespace qReal
{
namespace widgetsEdit
{

class RootWidget : public QWidget
{
	Q_OBJECT

public:
	RootWidget();
	void setShape(QDomDocument const &shape);
	void setElementRepo(ElementRepoInterface *repo);
	QDomDocument shapeDocument() const;

protected:
	virtual void paintEvent(QPaintEvent *);

private:
	QDomDocument mShape;
	SdfRenderer *mRenderer;
};

}
}
