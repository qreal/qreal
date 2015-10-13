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

#include <QtWidgets/QApplication>

class QMouseEvent;
class QWheelEvent;
class QDropEvent;
class QKeyEvent;

namespace qReal {

/// Class for QReal Application 
/// for interception all events of application for recording
/// mouse click coordinates.
class QRealApplication : public QApplication
{
	Q_OBJECT

public:
	QRealApplication(int & argc, char ** argv);

signals:
	/// This event may be subscribed to obtain all low-level user actions, timer events and everything about
	/// application`s event propagation subsystem. This is very powerful and dangerous tool, use it with care,
	/// it can strongly harm application`s performance.
	void lowLevelEvent(QObject *obj, QEvent *e);

protected:
	bool notify(QObject *receiver, QEvent *event);

private slots:
	void logMouse(QWidget * const target, QMouseEvent * const event);
	void logWheel(QWidget * const target, QWheelEvent * const event);
	void logDrop(QWidget * const target, QDropEvent * const event);
	void logKey(QKeyEvent * const event);

	void onFocusChanged(QWidget *old, QWidget *now);
};

}
