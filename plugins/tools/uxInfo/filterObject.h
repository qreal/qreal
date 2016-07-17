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

#include <QtCore/QObject>

namespace uxInfo {

/// Class for user experience information about
/// using menu elements and setting usability
/// testing mode and collection ux information mode.
class FilterObject : public QObject
{
	Q_OBJECT

public:
	explicit FilterObject(QObject *parent = 0);

public slots:
	/// Interception of all events for ux information.
	bool eventFilter(QObject *object, QEvent *event);

	/// Record information about triggered action.
	void triggeredActionActivated();

	/// Record information about toggled action.
	void toggledActionActivated(bool status);

	/// Change collection ux information mode.
	void setStatusCollectUsabilityStatistics(bool status);

	///Record about start of the usability test.
	void reportTestStarted();

	///Record about start of the usability test.
	void reportTestFinished();
};

}
