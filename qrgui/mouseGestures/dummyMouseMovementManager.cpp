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

#include "dummyMouseMovementManager.h"

#include <qrkernel/logging.h>

#include "gesturesWidget.h"
#include "private/pathCorrector.h"
#include "private/levenshteinDistance.h"

const QString comma = ", ";
const QString pointDelimeter = " : ";
const QString pathDelimeter = " | ";

const QString deletionGestureKey = "<deletionMetaGesture>";
/// @todo: specify it in metamodel?
const QString deletionGesture = "0, 200 : 200, 0 : ";

using namespace qReal::gestures;

DummyMouseMovementManager::DummyMouseMovementManager(const Id &diagram
		, const qReal::EditorManagerInterface &editorManagerInterface)
	: mDiagram(diagram)
	, mEditorManagerInterface(editorManagerInterface)
{
	mKeyStringManager.reset(new KeyManager);
	mGesturesManager.reset(new MixedGesturesManager);
}

QWidget *DummyMouseMovementManager::producePainter() const
{
	GesturesWidget * const result = new GesturesWidget;
	QList<QPair<QString, Id> > elements;
	for (const Id &element : mEditorManagerInterface.elements(mDiagram)) {
		if (!mEditorManagerInterface.mouseGesture(element).isEmpty()) {
			elements << qMakePair(mEditorManagerInterface.friendlyName(element), element);
		}
	}

	connect(result, &GesturesWidget::currentElementChanged
			, this, &DummyMouseMovementManager::drawIdealPath, Qt::QueuedConnection);
	result->setElements(elements);
	return result;
}

void DummyMouseMovementManager::drawIdealPath()
{
	GesturesWidget * const gesturesPainter = static_cast<GesturesWidget *>(sender());
	const Id currentElement = gesturesPainter->currentElement();
	if (mEditorManagerInterface.elements(mDiagram).contains(currentElement)) {
		const QString paths = mEditorManagerInterface.mouseGesture(currentElement);
		gesturesPainter->draw(paths);
	}
}

void DummyMouseMovementManager::clear()
{
	mPath.clear();
}

QLineF DummyMouseMovementManager::newLine()
{
	QLineF line;
	if (mPath.back().size() > 1) {
		line.setP1(mPath.back().at(mPath.back().size() - 2));
		line.setP2(mPath.back().back());
	}

	return line;
}

void DummyMouseMovementManager::initializeGestures()
{
}

void DummyMouseMovementManager::recountCentre()
{
	if (mPath.empty() || mPath.back().empty()) {
		return;
	}

	int count = 0;
	for (const PointVector &path : mPath) {
		count += path.size();
	}

	mCenter = ((count - 1) * mCenter + mPath.back().back()) / count;
}

void DummyMouseMovementManager::mousePress(const QPointF &pnt)
{
	QList<QPointF> path;
	path.push_back(pnt);
	mPath.push_back(path);
	recountCentre();
}

void DummyMouseMovementManager::mouseMove(const QPointF &pnt)
{
	PointVector path = mPath.back();
	mPath.pop_back();
	path.push_back(pnt);
	mPath.push_back(path);
	recountCentre();
}

QPointF DummyMouseMovementManager::pos()
{
	return mCenter;
}

QPoint DummyMouseMovementManager::parsePoint(const QString &str)
{
	bool isInt = true;
	int x = str.section(comma, 0, 0).toInt(&isInt, 0);
	int y = str.section(comma, 1, 1).toInt(&isInt, 0);
	return QPoint(x, y);
}

DummyMouseMovementManager::GestureResult DummyMouseMovementManager::result()
{
	return GestureResult();
}

QPointF DummyMouseMovementManager::firstPoint()
{
	if (!mPath.isEmpty() && !mPath.at(0).empty()) {
		return QPointF(mPath.at(0).at(0));
	}
	return QPointF(0, 0);
}

QPointF DummyMouseMovementManager::lastPoint()
{
	if (!mPath.isEmpty() && !mPath.back().empty()) {
		return QPointF(mPath.back().back());
	}
	return QPointF(0, 0);
}

bool DummyMouseMovementManager::wasMoving()
{
	return (firstPoint() != lastPoint() || mPath.size() > 1 ||
			(!mPath.isEmpty() && mPath.at(0).size() > 2));
}

bool DummyMouseMovementManager::isEdgeCandidate()
{
	return mPath.count() <= 1;
}

bool DummyMouseMovementManager::pathIsEmpty()
{
	return mPath.isEmpty();
}
