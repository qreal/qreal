/* Copyright 2016-2017 CyberTech Labs Ltd.
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

#include <trikControl/displayInterface.h>

#include <trikKitInterpreterCommon/robotModel/twoD/trikTwoDRobotModel.h>

namespace trik {

namespace robotModel {
namespace twoD {
namespace parts {
class Display;
}
}
}

class TrikDisplayEmu : public trikControl::DisplayInterface
{
	Q_OBJECT

	// DisplayInterface interface
public:
	TrikDisplayEmu(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model);
	virtual trikControl::DisplayWidgetInterface &graphicsWidget() override;

	void init();

public slots:
	virtual void showImage(const QString &fileName) override;
	virtual void addLabel(const QString &text, int x, int y) override;
	virtual void removeLabels() override {}
	virtual void setPainterColor(const QString &color) override;
	virtual void setPainterWidth(int penWidth) override;
	virtual void drawLine(int x1, int y1, int x2, int y2) override;
	virtual void drawPoint(int x, int y) override;
	virtual void drawRect(int x, int y, int width, int height, bool filled) override;
	virtual void drawEllipse(int x, int y, int width, int height, bool filled) override;
	virtual void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) override;
	virtual void setBackground(const QString &color) override;
	virtual void hide() override {}
	virtual void clear() override;
	virtual void reset() override;
	virtual void redraw() override;

private:
	trikControl::DisplayWidgetInterface mDisplayWidgetInterface;
	QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;
	robotModel::twoD::parts::Display * mDisplay;
};

}
