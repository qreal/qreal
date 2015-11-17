/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

#include <QtWidgets/QFrame>

#include <qrutils/utilsDeclSpec.h>

class QGraphicsItem;

namespace graphicsUtils {

class AbstractScene;

/// A base class for popup window that configures some item on graphicsUtils::AbstractScene.
/// Each time when user selets the item of template type this popup will appear to specify some
/// properties of the item.
class QRUTILS_EXPORT ItemPopup : public QFrame
{
	Q_OBJECT

public:
	explicit ItemPopup(AbstractScene &scene, QWidget *parent = 0);
	~ItemPopup();

	/// Enables or disables automatic popping up.
	void setEnabled(bool enabled);

protected slots:
	/// This will set fixed size of this window in consideration with layout.
	/// Must be called each time when layout of this widget changes.
	/// This is not performed automaticly cause this widget is not in parent layout, it floats
	/// on the parent widget.
	void updateDueToLayout();

	/// Sets the property to all items .
	/// Items that are not QObjects are ignored. Setting properties is performed with Qt reflection.
	void setPropertyMassively(const QString &property, const QVariant &value);

	/// Implementation must return should this window be shown for the given item or not.
	/// This can be prettily done with templates, but QObject can not be used with them.
	/// Default implementation returns true for graphicsUtils::AbstractItem instances.
	virtual bool suits(QGraphicsItem *item);

	/// Called when this window is shown for some item.
	/// If false is returned then the popup will not be shown.
	/// Default implementation calls attachTo({item}).
	virtual bool attachTo(QGraphicsItem *item);

	/// Called when this window is shown for some group of items.
	/// If false is returned then the popup will not be shown.
	/// Default implementation remembers items for future and returns true.
	virtual bool attachTo(const QList<QGraphicsItem *> &items);

	/// Called when user canceled his selection and before the window is hidden.
	/// Default implementation clears items memorized last time.
	virtual void detach();

signals:
	/// Emitted when user modifies some property or a group of properties via this popup window.
	void somethingChanged();

private slots:
	void onMousePressedScene();
	void onMouseReleasedScene();

	void checkSelection();

protected:
	void mousePressEvent(QMouseEvent *event) override;

	/// Searches for the given property value that is met most oftenly among items memorized in last attachTo() call.
	/// Items that are not QObjects are ignored. Obtaining property values is performed with Qt reflection.
	QVariant dominantPropertyValue(const QString &property) const;

	/// Returns true if at least one of currently selected items has \a property. Otherwise returns false.
	bool hasProperty(const QString &property) const;

	QPointF leftmostTopmost(const QList<QGraphicsItem *> &items);

	AbstractScene &mScene;
	QList<QGraphicsItem *> mCurrentItems;
	bool mEnabled = true;
	bool mMousePressed = false;
};

}
