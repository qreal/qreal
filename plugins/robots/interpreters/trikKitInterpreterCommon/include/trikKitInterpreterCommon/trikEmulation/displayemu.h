#pragma once

#include <trikControl/displayInterface.h>

#include <trikKitInterpreterCommon/robotModel/twoD/trikTwoDRobotModel.h>

namespace trik {

class TrikDisplayEmu : public trikControl::DisplayInterface {
	Q_OBJECT

	// DisplayInterface interface
public:
	TrikDisplayEmu(QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> model);
	virtual trikControl::DisplayWidgetInterface &graphicsWidget() override;

public slots:
	virtual void showImage(const QString &fileName) override;
	virtual void addLabel(const QString &text, int x, int y) override {}
	virtual void removeLabels() override {}
	virtual void setPainterColor(const QString &color) override {}
	virtual void setPainterWidth(int penWidth) override {}
	virtual void drawLine(int x1, int y1, int x2, int y2) override {}
	virtual void drawPoint(int x, int y) override {}
	virtual void drawRect(int x, int y, int width, int height) override {}
	virtual void drawEllipse(int x, int y, int width, int height) override {}
	virtual void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) override {}
	virtual void setBackground(const QString &color) override {}
	virtual void hide() override {}
	virtual void clear() override {}
	virtual void reset() override {}
	virtual void redraw() override {}

private:
	QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;

	bool isSmiling; // for debug

};

}
