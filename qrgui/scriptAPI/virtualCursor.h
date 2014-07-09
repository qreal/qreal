#pragma once

#include <QtWidgets/QLabel>
#include <QtCore/QString>
#include <QtWidgets/QComboBox>
#include <QtCore/QPropertyAnimation>

namespace qReal {

class MainWindow;

namespace gui {

class ScriptAPI;
class DraggableElement;

class VirtualCursor : public QLabel
{
	Q_OBJECT

public:
	explicit VirtualCursor(MainWindow *mainWindow, ScriptAPI *scriptAPI);

	///Moves virtual cursor to target with type for duration ms.
	Q_INVOKABLE void moveTo(QWidget const *target, int duration);

	///Moves virtual cursor to target coordinated with type for duration ms.
	Q_INVOKABLE void moveToRect(QRect target, int duration);

	///Moves virtual cursor to pos on scene with type for duration ms.
	Q_INVOKABLE void sceneMoveTo(QWidget *target, int duration, int xSceneCoord, int ySceneCoord);

	///Drag palette element to scene .
	Q_INVOKABLE QString dragPaletteElement(QString const &paletteElementId, int const duration, int const xSceneCoord, int const ySceneCoord);

	///post left mouse button press to widget.
	Q_INVOKABLE void leftButtonPress(QWidget *target, int delay = -1);

	///post left mouse button release to widget.
	Q_INVOKABLE void leftButtonRelease(QWidget *target, int delay = -1);

	///post left mouse button release to widget.
	Q_INVOKABLE void rightButtonPress(QWidget *target, int delay = -1);

	///post right mouse button release to widget.
	Q_INVOKABLE void rightButtonRelease(QWidget *target, int delay = -1);

	///draw link on scene.
	Q_INVOKABLE void drawLink(QString const &fromElementId, QString const &toElementId, int const duration);

	///Pick combo box item
	Q_INVOKABLE void pickComboBoxItem(QComboBox *comboBox, QString const &name, int const duration);

	///Emulate keyboard
	Q_INVOKABLE void type(QString const &message, int const duration);

protected:
	void paintEvent(QPaintEvent *event);

private slots:
	void virtualDrawing();
	void edgeFinished();

private:
	QString createBlockOnScene(DraggableElement const  *paletteElement, int const xSceneCoord, int const ySceneCoord);
	void attachPaletteElement(QIcon iconName);
	void detachPaletteElementIcon();
	void printValue(QString const &value, int const duration);
	void setPropertyComboBox(QRect target, int const value, int const property);

	MainWindow *mMainWindow;
	QIcon mPaletteElementIcon;
	ScriptAPI *mScriptAPI;
	QPropertyAnimation *mCursorMoveAnimation;

	bool mRightButtonPressed;
	bool mIsPaletteElementAttached;
};

}
}
