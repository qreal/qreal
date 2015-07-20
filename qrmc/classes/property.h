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

#include <QtCore/QString>

#include "../../qrrepo/repoApi.h"

namespace qrmc {
	class Property
	{
	public:
		Property(qrRepo::LogicalRepoApi *api, const qReal::Id &id);

		bool init();
		QString name() const;
		QString type() const;
		QString defaultValue() const;
		Property *clone();
		bool operator == (const Property &other) const;
		bool operator != (const Property &other) const;
		bool isReferenceProperty() const;
		void print() const;

		QString generatePropertyLine(const QString &lineTemplate) const;
		QString generateDefaultsLine(const QString &lineTemplate) const;
		QString generateDisplayedNameLine(const QString &lineTemplate) const;

	private:
		qrRepo::LogicalRepoApi *mApi;
		qReal::Id mId;
		QString mName;
		QString mType;
		bool mIsEnum;
		bool mIsReference;
		QString mDescription;
		QString mDefaultValue;
		QString mDisplayedName;
	};
}
