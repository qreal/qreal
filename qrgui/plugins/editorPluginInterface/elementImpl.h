/* Copyright 2007-2015 QReal Research Group
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

#include <QtWidgets/QWidget>
#include <QtCore/QList>
#include <QtCore/QRectF>
#include <QtCore/QPointF>
#include <QtGui/QPainter>

#include <qrkernel/ids.h>

#include "qrgui/plugins/editorPluginInterface/labelInterface.h"
#include "qrgui/plugins/editorPluginInterface/labelFactoryInterface.h"
#include "qrgui/plugins/editorPluginInterface/elementRepoInterface.h"
#include "qrgui/plugins/editorPluginInterface/sdfRendererInterface.h"
#include "qrgui/plugins/editorPluginInterface/portHelpers.h"

namespace enums {
namespace linkShape {
enum LinkShape
{
	broken
	, square
	, curve
};
}
}

namespace qReal {

typedef QPair<QPair<qReal::Id, qReal::Id>, QPair<bool, qReal::Id> > PossibleEdge;

/// @todo: wt*?!
typedef QPair<QPair<QString, QString>, QPair<bool, QString> > StringPossibleEdge;

typedef QPair<bool, qReal::Id> PossibleEdgeType;
typedef QPair<qReal::Id, qReal::Id> ElementPair;

/** @class ElementImpl
*	@brief base class for generated stuff in plugins
*	TODO: split into NodeElementImpl and EdgeElementImpl
* */

class ElementImpl
{
public:
	virtual ~ElementImpl() {}
	virtual void init(QRectF &contents, const PortFactoryInterface &portFactory, QList<PortInterface *> &ports
			, LabelFactoryInterface &labelFactory, QList<LabelInterface *> &labels
			, SdfRendererInterface *renderer, ElementRepoInterface *elementRepo = 0) = 0;
	virtual void init(LabelFactoryInterface &factory
			, QList<LabelInterface*> &titles) = 0;
	virtual void paint(QPainter *painter, QRectF &contents) = 0;
	virtual void updateData(ElementRepoInterface *repo) const = 0;
	virtual bool isNode() const = 0;
	virtual bool isResizeable() const = 0;
	virtual Qt::PenStyle getPenStyle() const = 0;
	virtual int getPenWidth() const = 0;
	virtual QColor getPenColor() const = 0;
	virtual void drawStartArrow(QPainter *painter) const = 0;
	virtual void drawEndArrow(QPainter *painter) const = 0;
	virtual bool isDividable() const = 0;

	/*Container properties*/
	virtual bool isContainer() const = 0;
	virtual bool isSortingContainer() const = 0;
	virtual QVector<int> sizeOfForestalling() const = 0;
	virtual int sizeOfChildrenForestalling() const = 0;
	virtual bool hasMovableChildren() const = 0;
	virtual bool minimizesToChildren() const = 0;
	virtual bool maximizesChildren() const = 0;

	virtual QStringList fromPortTypes() const = 0;
	virtual QStringList toPortTypes() const = 0;

	virtual enums::linkShape::LinkShape shapeType() const = 0;

	virtual bool createChildrenFromMenu() const = 0;

	virtual QList<qreal> border() const = 0;

	/// Update shape of an element. Does nothing in case of generated editors, used by metamodel interpreter.
	virtual void updateRendererContent(const QString &shape)
	{
		Q_UNUSED(shape);
	}
};

}
