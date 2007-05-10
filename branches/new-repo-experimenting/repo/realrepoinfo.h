#ifndef REALREPOINFO_H
#define REALREPOINFO_H

#include <QStringList>

class RealRepoInfo
{
	public:
		RealRepoInfo();
		~RealRepoInfo();

		QStringList getObjectCategories() const;
		QStringList getObjects(int category) const;
	private:
		QStringList m_categories;
};

#endif
