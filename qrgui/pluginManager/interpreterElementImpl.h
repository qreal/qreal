#pragma once

#include <QtCore/QMap>
#include <QtWidgets/QWidget>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QRectF>
#include <QtCore/QPointF>
#include <QtGui/QPainter>

#include "../editorPluginInterface/elementImpl.h"
#include "../editorPluginInterface/elementTitleHelpers.h"
#include "../editorPluginInterface/elementRepoInterface.h"
#include "../umllib/sdfRenderer.h"
#include "../../qrkernel/ids.h"
#include "../../qrrepo/repoApi.h"
#include "../../qrxc/graphicType.h"

typedef QPair<QPair<qReal::Id, qReal::Id>, QPair<bool, qReal::Id> > PossibleEdge;
typedef QPair<QPair<QString, QString>, QPair<bool, QString> > StringPossibleEdge;
typedef QPair<bool, qReal::Id> PossibleEdgeType;

namespace qReal {

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
	void init(QRectF &contents, QList<StatPoint> &pointPorts
			, QList<StatLine> &linePorts, LabelFactoryInterface &factory
			, QList<LabelInterface*> &titles
			, SdfRendererInterface *renderer, SdfRendererInterface *portRenderer
			, ElementRepoInterface *elementRepo = 0);
	void init(LabelFactoryInterface &factory
			, QList<LabelInterface*> &titles);
	void paint(QPainter *painter, QRectF &contents);
	void updateData(ElementRepoInterface *repo) const;
	bool isNode() const;
	bool hasPorts() const;
	bool isResizeable() const;
	Qt::PenStyle getPenStyle() const;
	int getPenWidth() const;
	QColor getPenColor() const;
	void drawStartArrow(QPainter *painter) const;
	void drawEndArrow(QPainter *painter) const;

	bool isDividable() const;

	/*Container properties*/
	bool hasContainerProperty(QString const &property) const;
	bool isContainer() const;
	bool isSortingContainer() const;
	QVector<int> sizeOfForestalling() const;
	int sizeOfChildrenForestalling() const;
	bool hasMovableChildren() const;
	bool minimizesToChildren() const;
	bool maximizesChildren() const;

	bool isPort() const;
	bool hasPin() const;

	QList<double> border() const;

	QStringList bonusContextMenuFields() const;
	void updateRendererContent(QString const &shape);

private:
	void initPointPorts(QList<StatPoint> &pointPorts, QDomDocument &portsDoc, QDomNode &portsPicture, int const &width, int const &height);
	void initLinePorts(QList<StatLine> &linePorts, QDomDocument &portsDoc, QDomNode &portsPicture, int const &width, int const &height);
	void initLabels(int const &width, int const &height, LabelFactoryInterface &factory, QList<LabelInterface*> &titles);
	QVector<int> getSizeOfContainerProperty(QString const &property) const;
	QStringList getListOfStr(QString const &labelText) const;
	QString getResultStr(QStringList const &list, ElementRepoInterface *repo) const;
	void drawArrow(QPainter *painter, QString const &type) const;

	qrRepo::RepoApi *mEditorRepoApi;  // Doesn't have ownership.
	Id mId;
	SdfRendererInterface *mRenderer;  // Doesn't have ownership.
	QDomDocument mGraphics;
	QList<NodeLabel> mNodeLabels;
	QList<EdgeLabel> mEdgeLabels;
};
}

