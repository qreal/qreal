#pragma once

#include <QtWidgets/QLabel>
#include <QtCore/QString>
#include <QtWidgets/QComboBox>
#include <QtCore/QPropertyAnimation>

namespace qReal {

class MainWindow;

namespace gui {

class ScriptAPI;

/// This class provides tool for emulate mouse actions. Represented as QLabel with some logic same as real
/// mouse. Virtual Mouse post some QMouseEvents to system, when working.
/// Can work in "hard" and "light" modes, whos
/// difference is in posted move events. Hard mode needed in some methods in API. mRightButtonPressed
/// is true in this case.

class VirtualCursor : public QLabel
{
	Q_OBJECT

public:
	explicit VirtualCursor(ScriptAPI *scriptAPI, QWidget *parent);

	///Moves virtual cursor to target with type for duration ms.
	Q_INVOKABLE void moveTo(QWidget const *target, int duration);

	///Moves virtual cursor to target coordinated with type for duration ms.
	Q_INVOKABLE void moveToRect(QRect target, int duration);

	///Moves virtual cursor to pos on scene with type for duration ms.
	Q_INVOKABLE void sceneMoveTo(QWidget *target, int duration, int xSceneCoord, int ySceneCoord);

	///Post left mouse button press to widget.
	Q_INVOKABLE void leftButtonPress(QWidget *target, int delay = -1);

	///Post left mouse button release to widget.
	Q_INVOKABLE void leftButtonRelease(QWidget *target, int delay = -1);

	///Post left mouse button release to widget.
	Q_INVOKABLE void rightButtonPress(QWidget *target, int delay = -1);

	///post right mouse button release to widget.
	Q_INVOKABLE void rightButtonRelease(QWidget *target, int delay = -1);

	///Post mouse move event to widget.
	Q_INVOKABLE void moved(QWidget *target);

	///Draw palette element icon near virtual cursor by name.
	void attachPaletteElement(QIcon iconName);

	///Redraw virtual cursor without ant icon.
	void detachPaletteElementIcon();

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void edgeFinished();

	QIcon mPaletteElementIcon;
	ScriptAPI *mScriptAPI;
	QPropertyAnimation *mCursorMoveAnimation;

	bool mRightButtonPressed;
	bool mIsPaletteElementAttached;
};

}
}
