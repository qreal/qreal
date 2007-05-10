#include <QStringList>

class Category{
	public:
		QString name;
	QList<int> objects;
};

class RealRepoInfo
{public:
	RealRepoInfo();
	~RealRepoInfo();
	QStringList getObjectCategories() const;
	QList<int> getObjects(int category) const;
	QString objectDesc(int id) const;
	QString objectName(int id) const;

private:
	QList< Category > categories;
	QStringList objects;
	QStringList descriptions;
	QStringList m_categories;
};
