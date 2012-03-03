#include "../editorPluginInterface/elementImpl.h"

class Beep : public ElementImpl
{
public:
	void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles);
	void init(QRectF &contents, QList<StatPoint> &pointPorts,
						QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
						QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
						SdfRendererInterface *portRenderer);
	 ElementImpl *clone();
	~Beep();

	void paint(QPainter *painter, QRectF &contents);
	Qt::PenStyle getPenStyle();
	int getPenWidth();
	QColor getPenColor();
	void drawStartArrow(QPainter *) const;
	void drawEndArrow(QPainter *) const;
	bool hasPorts();
	void updateData(ElementRepoInterface *repo) const;
	bool isNode();
	bool isResizeable();
	bool isContainer();
	bool isSortingContainer();
	int sizeOfForestalling();
	int sizeOfChildrenForestalling();
	bool hasMovableChildren();
	bool minimizesToChildren();
	bool maximizesChildren();
	bool isPort();
	bool hasPin();
	QList<double> border();
	QStringList bonusContextMenuFields();

private:
	SdfRendererInterface *mRenderer;
	ElementTitleInterface *title_1;
	ElementTitleInterface *title_2;
};

