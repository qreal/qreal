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

#include <QtWidgets/QGraphicsTextItem>

#include "plugins/editorPluginInterface/labelInterface.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {

class Label : public QGraphicsTextItem, public LabelInterface
{
	Q_OBJECT

public:
	Label(models::GraphicalModelAssistApi &graphicalAssistApi, const Id &elementId
			, int index, qreal x, qreal y, const QString &text, qreal rotation);

	Label(models::GraphicalModelAssistApi &graphicalAssistApi, const Id &elementId
			, int index, qreal x, qreal y, const QString &binding, bool readOnly, qreal rotation);

	~Label() override;

	void init(const QRectF &contents);
	void setBackground(const QColor &background);
	void setScaling(bool scalingX, bool scalingY);

	bool isHard() const;
	virtual void setHard(bool hard);

	bool isReadOnly() const;

	void startTextInteraction();
	void setTitleFont();

	void setTextFromRepo(const QString& text);

	void setParentSelected(bool isSelected);
	void setParentContents(const QRectF &contents);

	void setShouldCenter(bool shouldCenter);
	void scaleCoordinates(const QRectF &contents);

	void clearMoveFlag();

	virtual void setFlags(GraphicsItemFlags flags);
	virtual void setTextInteractionFlags(Qt::TextInteractionFlags flags);
	virtual void setHtml(const QString &html);
	virtual void setPlainText(const QString &text);

protected:
	enum InterpreterPropertyType
	{
		propertyText
		, coordinate
		, textWidth
	};

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	virtual void focusOutEvent(QFocusEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

private:
	void init();
	void updateData(bool withUndoRedo = false);
	void updateRect(QPointF newBottomRightPoint);
	void setText(const QString &text);
	void moveToParentCenter();
	Qt::Orientation orientation();
	QRectF labelMovingRect() const;

	QString enumText(const QString &enumValue) const;

	bool mFocusIn;
	bool mReadOnly;
	bool mScalingX;
	bool mScalingY;
	QMap<QString, QString> mEnumValues;
	QRectF mContents;
	QRectF mParentContents;
	qreal mRotation;
	QPointF mPoint;
	QString mOldText;
	QString mBinding;
	QColor mBackground;
	bool mIsStretched;
	bool mIsHard;
	bool mParentIsSelected;
	bool mWasMoved;
	bool mShouldMove;
	const int mIndex;
	const Id mId;
	models::GraphicalModelAssistApi &mGraphicalModelAssistApi;
};

}
