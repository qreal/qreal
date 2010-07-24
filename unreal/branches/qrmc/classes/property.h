#pragma once

#include <QtCore/QString>

#include "../../../trunk/qrrepo/repoApi.h"

class Property
{
public:
	Property(qrRepo::RepoApi *api, qReal::Id const &id);

	bool init();
	QString name();
	QString type();
	QString defaultValue();
	Property *clone();
	bool operator == (Property const &other) const;
	bool operator != (Property const &other) const;
	void print() const;

private:
	qrRepo::RepoApi *mApi;
	qReal::Id mId;
	QString mName;
	QString mType;
	bool mIsEnum;
	bool mIsReference;
	QString mDescription;
	QString mDefaultValue;
};
