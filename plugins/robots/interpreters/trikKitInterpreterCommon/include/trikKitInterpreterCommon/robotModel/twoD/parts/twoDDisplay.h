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

#include <QtGui/QColor>

#include <utils/canvas/canvas.h>
#include <trikKit/robotModel/parts/trikDisplay.h>
#include <twoDModel/engine/twoDModelEngineInterface.h>
#include <twoDModel/engine/twoDModelDisplayInterface.h>

#include "trikKitInterpreterCommon/declSpec.h"

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT Display : public robotModel::parts::TrikDisplay, public utils::Canvas
{
	Q_OBJECT
	// Canvas cannot be QObject because of ambiguous base so we are forced to copy properties here.
	Q_PROPERTY(QList<utils::CanvasObject *> objects READ objects)
	Q_PROPERTY(QList<utils::PointObject *> pixels READ pixels)
	Q_PROPERTY(QList<utils::LineObject *> segments READ segments)
	Q_PROPERTY(QList<utils::RectangleObject *> rectangles READ rectangles)
	Q_PROPERTY(QList<utils::EllipseObject *> ellipses READ ellipses)
	Q_PROPERTY(QList<utils::ArcObject *> arcs READ arcs)
	Q_PROPERTY(QList<utils::TextObject *> labels READ labels)
	Q_PROPERTY(QString background READ background WRITE setBackground)
	Q_PROPERTY(bool smiles READ smiles)
	Q_PROPERTY(bool sadSmiles READ sadSmiles)

public:
	Display(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	/// Returns the name of the current background color.
	QString background() const;

	/// Returns true if robot draws happy smile at the moment.
	bool smiles() const;

	/// Returns true if robot draws sad smile at the moment.
	bool sadSmiles() const;

	void drawSmile(bool sad) override;
	void setBackground(const QColor &color) override;
	void clearScreen() override;
	void setPainterColor(const QColor &color) override;
	void setPainterWidth(int penWidth) override;
	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height)  override;
	void drawEllipse(int x, int y, int width, int height) override;
	void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) override;
	void printText(int x, int y, const QString &text) override;
	void paint(QPainter *painter) override;
	void reset() override;
	void redraw() override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
	QColor mBackground;
	QImage mCurrentImage;
	bool mSmiles;
	bool mSadSmiles;
	/// @todo: QPoint can`t be used in map without operators declaration.
	QHash<QPair<int, int>, utils::TextObject *> mLabelsMap;
};

}
}
}
}
