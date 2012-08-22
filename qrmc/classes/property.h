#pragma once

#include <QtCore/QString>

#include "../../qrrepo/repoApi.h"

namespace qrmc {
	class Property
	{
	public:
		Property(qrRepo::LogicalRepoApi *api, qReal::Id const &id);

		bool init();
		QString name();
		QString type();
		QString defaultValue();
		Property *clone();
		bool operator == (Property const &other) const;
		bool operator != (Property const &other) const;
		void print() const;

		QString generatePropertyLine(QString const &lineTemplate) const;
		QString generateDefaultsLine(QString const &lineTemplate) const;
		QString generateDisplayedNameLine(QString const &lineTemplate) const;

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
