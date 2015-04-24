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

#include <QtWidgets/QGraphicsItem>

class QDomElement;
class QGraphicsTextItem;

namespace twoDModel {
namespace items {

/// Represents some zone on the 2D model world, probably with some text like "Start zone".
class RegionItem : public QGraphicsObject
{
	Q_OBJECT

public:
	explicit RegionItem(QGraphicsItem *parent = 0);

	/// Returns a unique identifier of the region.
	QString id() const;

	/// Sets a unique identifier of the region.
	void setId(const QString &id);

	/// Returns true if the region is filled with hatching.
	bool filled() const;

	/// Enables or disables filling region with hatching.
	void setFilled(bool filled);

	/// Returns text that written over the region.
	QString text() const;

	/// Sets text that written over the region.
	void setText(const QString &text);

	/// Returns the upper-left corner of the text relatively to upper-left corner of the item.
	QPointF textPosition() const;

	/// Sets the upper-left corner of the text relatively to upper-left corner of the item.
	void setTextPosition(const QPointF &pos);

	/// Returns the color of the item`s borders, text and hatching.
	QColor color() const;

	/// Sets the color of the item`s borders, text and hatching.
	void setColor(const QColor &color);

	/// Sets the size of an item on the scene.
	void setSize(const QSizeF &size);

	/// Returns true if the given point in scene coordinates is contained by this region.
	bool containsPoint(const QPointF &point) const;

	/// Returns true if the center of the bounding rect of the given item is contained by this region.
	bool containsItem(QGraphicsItem *item) const;

	QRectF boundingRect() const override;

	virtual void serialize(QDomElement &element);
	virtual void deserialize(const QDomElement &element);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
	virtual QString regionType() const = 0;

	QPointF deserializePoint(const QDomElement &element, const QString &xAttribute, const QString &yAttribute);

	QGraphicsTextItem *mTextItem;  // Takes ownership
	QString mId;
	bool mFilled;
	QColor mColor;
	QSizeF mSize;
};

}
}
