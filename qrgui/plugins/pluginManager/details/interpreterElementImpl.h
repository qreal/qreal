#pragma once

#include <QtCore/QMap>
#include <QtWidgets/QWidget>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QRectF>
#include <QtCore/QPointF>
#include <QtGui/QPainter>

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>

#include "plugins/editorPluginInterface/elementImpl.h"
#include "plugins/editorPluginInterface/labelInterface.h"
#include "plugins/editorPluginInterface/elementRepoInterface.h"

#include "plugins/pluginManager/sdfRenderer.h"

namespace qReal {

typedef QPair<QPair<qReal::Id, qReal::Id>, QPair<bool, qReal::Id> > PossibleEdge;
typedef QPair<QPair<QString, QString>, QPair<bool, QString> > StringPossibleEdge;
typedef QPair<bool, qReal::Id> PossibleEdgeType;

struct EdgeLabel {
	QString labelText;
	QString labelType;
	LabelInterface* title;

	EdgeLabel(QString const &labelText_, QString const &labelType_, LabelInterface* title_)
		: labelText(labelText_), labelType(labelType_), title(title_)
	{}
};

struct NodeLabel {
	QString textBinded;
	QString center;
	LabelInterface* title;

	NodeLabel(QString const &textBinded_, QString const &center_, LabelInterface* title_)
		: textBinded(textBinded_), center(center_), title(title_)
	{}
};

/// This class is for implementation the graphical representation of element
class InterpreterElementImpl : public ElementImpl
{
public:
	InterpreterElementImpl(qrRepo::RepoApi *repo, Id const &metaId);
	void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
			, LabelFactoryInterface &labelFactory, QList<LabelInterface *> &labels
			, SdfRendererInterface *renderer, ElementRepoInterface *elementRepo = 0);
	void init(LabelFactoryInterface &labelFactory, QList<LabelInterface *> &labels);
	void paint(QPainter *painter, QRectF &contents);
	void updateData(ElementRepoInterface *repo) const;
	bool isNode() const;
	bool isResizeable() const;
	Qt::PenStyle getPenStyle() const;
	int getPenWidth() const;
	QColor getPenColor() const;
	void drawStartArrow(QPainter *painter) const;
	void drawEndArrow(QPainter *painter) const;

	bool isDividable() const;

	void inheritProperties(QList<QDomElement> &elements, const Id &id, QList<PortInterface *> &ports
			, const PortFactoryInterface &portFactory, SdfRendererInterface *renderer
			, LabelFactoryInterface &labelFactory, QList<LabelInterface *> &labels) const;

	/*Container properties*/
	bool hasContainerProperty(QString const &property) const;
	bool isContainer() const;
	bool isSortingContainer() const;
	QVector<int> sizeOfForestalling() const;
	int sizeOfChildrenForestalling() const;
	bool hasMovableChildren() const;
	bool minimizesToChildren() const;
	bool maximizesChildren() const;

	QStringList fromPortTypes() const;
	QStringList toPortTypes() const;

	enums::linkShape::LinkShape shapeType() const;

	bool createChildrenFromMenu() const;

	QList<qreal> border() const;

	QStringList bonusContextMenuFields() const;
	void updateRendererContent(QString const &shape);

private:
	void initPointPorts(PortFactoryInterface const &factory, QList<PortInterface *> &ports
			, int const &width, int const &height);
	void initLinePorts(PortFactoryInterface const &factory, QList<PortInterface *> &ports
			, int const &width, int const &height);
	void initLabels(int const &width, int const &height, LabelFactoryInterface &factory, QList<LabelInterface*> &titles);
	QVector<int> getSizeOfContainerProperty(QString const &property) const;
	QStringList getListOfStr(QString const &labelText) const;
	QString getResultStr(QStringList const &list, ElementRepoInterface *repo) const;
	void drawArrow(QPainter *painter, QString const &type) const;

	enums::linkShape::LinkShape shapeTypeByString(QString const &type) const;

	qrRepo::RepoApi *mEditorRepoApi;  // Doesn't have ownership.
	Id mId;
	SdfRendererInterface *mRenderer;  // Doesn't have ownership.
	QDomDocument mGraphics;
	QList<NodeLabel> mNodeLabels;
	QList<EdgeLabel> mEdgeLabels;
};
}

