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

#include <qrkernel/ids.h>

#include <QtCore/QMap>
#include <QtWidgets/QWidget>

#include "qrgui/mouseGestures/mouseGesturesDeclSpec.h"

namespace qReal {
namespace gestures {

class QRGUI_MOUSE_GESTURES_EXPORT MouseMovementManagerInterface : public QObject
{
	Q_OBJECT

public:
	/// Specifies the type of the result of the gesture made by user: regular element creation or some metagesture
	/// (e.g. element deletion).
	enum GestureResultType
	{
		invalidGesture
		, createElementGesture
		, deleteGesture
	};

	/// Specifies the result of the gesture made.
	struct QRGUI_MOUSE_GESTURES_EXPORT GestureResult
	{
	public:
		GestureResult()
			: mType(invalidGesture)
		{
		}

		explicit GestureResult(GestureResultType type, const Id &id = Id())
			: mType(type)
			, mId(id)
		{
		}

		/// Returns the resulting type of the gesture.
		GestureResultType type() const
		{
			return mType;
		}

		/// Returns the type of the element requested by the gesture.
		Id elementType() const
		{
			return mId;
		}

		/// Returns the resulting type of the gesture.
		void setType(GestureResultType type)
		{
			mType = type;
		}

		/// Sets the type of the element requested by the gesture.
		void setElementType(const Id &id)
		{
			mId = id;
		}

	private:
		GestureResultType mType;
		Id mId;
	};

	/// Creates and returns the widget that shows available for this manager mouse gestures.
	/// Transfers ownership.
	virtual QWidget *producePainter() const = 0;

	virtual void initializeGestures() = 0;
	virtual void mousePress(const QPointF &point) = 0;
	virtual void mouseMove(const QPointF &point) = 0;
	virtual void clear() = 0;
	virtual GestureResult result() = 0;
	virtual QPointF pos() = 0;
	virtual QPointF firstPoint() = 0;
	virtual QPointF lastPoint() = 0;
	virtual QLineF newLine() = 0;
	virtual bool wasMoving() = 0;
	virtual bool isEdgeCandidate() = 0;
	virtual bool pathIsEmpty() = 0;
};

}
}
