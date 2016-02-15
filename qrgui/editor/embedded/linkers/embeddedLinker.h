/* Copyright 2007-2016 QReal Research Group
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

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>

#include <qrkernel/roles.h>

#include "editor/edgeElement.h"
#include "models/commands/createElementsCommand.h"

namespace qReal {
namespace gui {
namespace editor {

class NodeElement;

class EmbeddedLinker : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	EmbeddedLinker();
	EmbeddedLinker(QGraphicsItem *parent);
	virtual ~EmbeddedLinker();

	void initTitle();
	void generateColor();

	bool isDirected() const;
	Id edgeType() const;
	NodeElement* master() const;

	void setDirected(const bool directed);
	void setMaster(NodeElement* const master);
	void setEdgeType(const Id & edgeType);

	void takePosition(int index, int maxIndex);

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w);

	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
	EdgeElement* mEdge;
	NodeElement* mMaster;
	Label* mTitle;

	float mSize;
	float mIndent;
	QColor mColor;
	QRectF mRectangle;
	QRectF mInnerRectangle;

	bool mDirected;
	bool mPressed;
	Id mEdgeType;

	QPointF mInitialClickPoint;
	qReal::commands::CreateElementsCommand *mCreateEdgeCommand;
};

}
}
}
