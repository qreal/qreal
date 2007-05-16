#include <QStringList>
#include <QMap>
#include <QString>

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

private:
};
