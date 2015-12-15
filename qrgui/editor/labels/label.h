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

#include <qrgui/plugins/editorPluginInterface/labelInterface.h>
#include <qrgui/models/graphicalModelAssistApi.h>

#include "qrgui/editor/labels/labelProperties.h"

namespace qReal {
namespace gui {
namespace editor {

class Label : public QGraphicsTextItem, public LabelInterface
{
	Q_OBJECT

public:
	Label(models::GraphicalModelAssistApi &graphicalAssistApi
			, const Id &elementId
			, const LabelProperties &properties);

	~Label() override;

	void init(const QRectF &contents);

	void setBackground(const QColor &background) override;
	void setScaling(bool scalingX, bool scalingY) override;
	void setHard(bool hard) override;

	void setFlags(GraphicsItemFlags flags) override;

	void setTextInteractionFlags(Qt::TextInteractionFlags flags) override;
	void setHtml(const QString &html) override;
	void setPlainText(const QString &text) override;

	void setPrefix(const QString &text) override;
	void setSuffix(const QString &text) override;

	void setTextFromRepo(const QString &text) override;

	QRectF boundingRect() const override;

	bool isReadOnly() const;
	bool isHard() const;

	void setParentSelected(bool isSelected);
	void setParentContents(const QRectF &contents);

	void setShouldCenter(bool shouldCenter);
	void scaleCoordinates(const QRectF &contents);

	void clearMoveFlag();

	void startTextInteraction();

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	void focusOutEvent(QFocusEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	void drawText(QPainter *painter, const QRectF &rect, const QString &text);
	QRectF prefixRect() const;
	QRectF suffixRect() const;
	QRectF textRect(const QString &text) const;

	void init();
	void reinitFont();

	void updateData(bool withUndoRedo = false);
	void updateRect(QPointF newBottomRightPoint);
	void setText(const QString &text);
	void moveToParentCenter();
	Qt::Orientation orientation();
	QRectF labelMovingRect() const;

	QString enumText(const QString &enumValue) const;

	QMap<QString, QString> mEnumValues;
	QRectF mContents;
	QRectF mParentContents;
	QString mOldText;
	bool mIsStretched;
	bool mWasMoved;
	bool mShouldMove;
	const Id mId;
	models::GraphicalModelAssistApi &mGraphicalModelAssistApi;
	LabelProperties mProperties;
};

}
}
}
