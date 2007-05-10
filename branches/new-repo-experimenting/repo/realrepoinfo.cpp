#include "realrepoinfo.h"

RealRepoInfo::RealRepoInfo()
{
	m_categories << "Use Case";
}

RealRepoInfo::~RealRepoInfo()
{
}

QStringList RealRepoInfo::getObjectCategories() const
{
	return m_categories;
}

QStringList RealRepoInfo::getObjects(int category) const
{
	QStringList blah;
	blah << "uscnActor" << "uscClassifier" << "uscExtensionPoint" << "uscUseCase" << "uscExtend" << "uscInclude";
	return blah;
}

