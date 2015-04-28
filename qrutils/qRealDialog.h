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

#include <QtWidgets/QDialog>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// A base class for all system dialogs. Provides such tools like automatic
/// serialization of dialog`s position and size
class QRUTILS_EXPORT QRealDialog : public QDialog
{
	Q_OBJECT

public:
	/// @param id The name of this dialog. Used for storing dialog`s parameters
	///in settings so should be unique.
	explicit QRealDialog(const QString &id, QWidget *parent = 0);

	/// After this method called till the next call of resumeSerialization() this instance will
	/// not serialize window geometry into settings when closed (default behaviour).
	void suspendSerialization();

	/// Cancels call of suspendSerialization(). After this method called window geometry will be
	/// serialized into settings again.
	void resumeSerialization();

protected:
	virtual void showEvent(QShowEvent *);
	virtual void closeEvent(QCloseEvent *);

	void serializeParameters();
	void deserializeParameters();

	/// Returns a key string used for storing maximized parameter in registry
	virtual QString maximizedKey() const;

	/// Returns a key string used for storing position parameter in registry
	virtual QString positionKey() const;

	/// Returns a key string used for storing size parameter in registry
	virtual QString sizeKey() const;

private:
	const QString mId;
	bool mSerializationSuspended = false;
};

}
