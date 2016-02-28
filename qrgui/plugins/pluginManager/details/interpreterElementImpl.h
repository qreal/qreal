/* Copyright 2012-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

#include <metaMetaModel/elementType.h>
#include <metaMetaModel/linkShape.h>
#include <metaMetaModel/elementRepoInterface.h>

#include "plugins/pluginManager/sdfRenderer.h"

namespace qReal {

typedef QPair<QPair<qReal::Id, qReal::Id>, QPair<bool, qReal::Id> > PossibleEdge;
typedef QPair<QPair<QString, QString>, QPair<bool, QString> > StringPossibleEdge;
typedef QPair<bool, qReal::Id> PossibleEdgeType;

//struct EdgeLabel {
//	QString labelText;
//	QString labelType;
//	LabelInterface *title;

//	EdgeLabel(const QString &labelText_, const QString &labelType_, LabelInterface* title_)
//		: labelText(labelText_), labelType(labelType_), title(title_)
//	{}
//};

//struct NodeLabel {
//	QString textBinded;
//	QString center;
//	LabelInterface *title;

//	NodeLabel(const QString &textBinded_, const QString &center_, LabelInterface* title_)
//		: textBinded(textBinded_), center(center_), title(title_)
//	{}
//};

/// This class is for implementation the graphical representation of element
class InterpreterElementImpl : public ElementType
{
public:
	InterpreterElementImpl(Metamodel &metamodel, qrRepo::RepoApi *repo, const Id &metaId);
//	void init(QRectF &contents, const PortFactoryInterface &portFactory, QList<PortInterface *> &ports
//			, LabelFactoryInterface &labelFactory, QList<LabelInterface *> &labels
//			, SdfRendererInterface *renderer, ElementRepoInterface *elementRepo = 0);
//	void init(LabelFactoryInterface &labelFactory, QList<LabelInterface *> &labels);
//	void paint(QPainter *painter, QRectF &contents);
//	void updateData(ElementRepoInterface *repo) const;
	bool isNode() const;
	bool isResizeable() const;
	Qt::PenStyle getPenStyle() const;
	int getPenWidth() const;
	QColor getPenColor() const;
	void drawStartArrow(QPainter *painter) const;
	void drawEndArrow(QPainter *painter) const;

	bool isDividable() const;

//	void inheritProperties(QList<QDomElement> &elements, const Id &id, QList<PortInterface *> &ports
//			, const PortFactoryInterface &portFactory, SdfRendererInterface *renderer
//			, LabelFactoryInterface &labelFactory, QList<LabelInterface *> &labels) const;

	/*Container properties*/
	bool hasContainerProperty(const QString &property) const;
	bool isContainer() const;
	bool isSortingContainer() const;
	QVector<int> sizeOfForestalling() const;
	int sizeOfChildrenForestalling() const;
	bool hasMovableChildren() const;
	bool minimizesToChildren() const;
	bool maximizesChildren() const;

	QStringList fromPortTypes() const;
	QStringList toPortTypes() const;

	LinkShape shapeType() const;

	bool createChildrenFromMenu() const;

	QList<qreal> border() const;

	void updateRendererContent(const QString &shape);

private:
//	void initPointPorts(const PortFactoryInterface &factory, QList<PortInterface *> &ports
//			, const int &width, const int &height);

//	void initLinePorts(const PortFactoryInterface &factory, QList<PortInterface *> &ports
//			, const int &width, const int &height);

//	void initLabels(const int &width, const int &height, LabelFactoryInterface &factory
//			, QList<LabelInterface*> &titles);

	QVector<int> getSizeOfContainerProperty(const QString &property) const;
	QStringList getListOfStr(const QString &labelText) const;
	QString getResultStr(const QStringList &list, ElementRepoInterface *repo) const;
	void drawArrow(QPainter *painter, const QString &type) const;

	LinkShape shapeTypeByString(const QString &type) const;

	Metamodel &mMetamodel;
	qrRepo::RepoApi *mEditorRepoApi;  // Doesn't have ownership.
	Id mId;
	SdfRendererInterface *mRenderer;  // Doesn't have ownership.
	QDomDocument mGraphics;
//	QList<NodeLabel> mNodeLabels;
//	QList<EdgeLabel> mEdgeLabels;
};
}

