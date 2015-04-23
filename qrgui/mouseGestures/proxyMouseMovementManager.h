#pragma once

#include <qrkernel/ids.h>

#include <QtCore/QMap>
#include <QtWidgets/QWidget>

#include "qrgui/mouseGestures/mouseGesturesDeclSpec.h"
#include "qrgui/mouseGestures/mouseMovementManagerInterface.h"

#include "private/keyManager.h"
#include "private/mixedgesturesmanager.h"
#include "private/geometricForms.h"

#include "plugins/pluginManager/editorManagerInterface.h"

namespace qReal {
namespace gestures {

class QRGUI_MOUSE_GESTURES_EXPORT ProxyMouseMovementManager : public MouseMovementManagerInterface
{
	Q_OBJECT

public:
	ProxyMouseMovementManager(const Id &diagram, const EditorManagerInterface &editorManagerInterface);

	/// Creates and returns the widget that shows available for this manager mouse gestures.
	/// Transfers ownership.
	QWidget *producePainter() const override;

	void initializeGestures() override;
	void mousePress(const QPointF &point) override;
	void mouseMove(const QPointF &point) override;
	void clear() override;
	GestureResult result() override;
	QPointF pos() override;
	QPointF firstPoint() override;
	QPointF lastPoint() override;
	QLineF newLine() override;
	bool wasMoving() override;
	bool isEdgeCandidate() override;
	bool pathIsEmpty() override;

private:
	static QPoint parsePoint(const QString &str);
	void recountCentre();
	void drawIdealPath();

	const Id mDiagram;
	const EditorManagerInterface &mEditorManagerInterface;
	PathVector mPath;
	QPointF mCenter;
	QSet<Id> mInitializedGestures;
	QScopedPointer<KeyManager> mKeyStringManager;
	QScopedPointer<GesturesManager> mGesturesManager;
};

}
}
