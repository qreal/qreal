#ifndef REALREPOINFO_H
#define REALREPOINFO_H

#include <QStringList>
#include <QMap>
#include <QString>
#include <QIcon>

class RealRepoInfo
{
public:
	RealRepoInfo();
	~RealRepoInfo();
	QStringList getObjectCategories() const;
	QList<int> getObjects(int category) const;
	QString objectDesc(int id) const;
	QString objectName(int id) const;

	QStringList getColumnNames(int type) const;
	QString getColumnName(int type, int role) const;

	int roleByIndex(int index) const
		{ return index+129; };
	int indexByRole(int role) const
		{ return role-129; };

	QIcon objectIcon( int id ) const; 

private:
};

#endif

