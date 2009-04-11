#include "ids.h"

using namespace qReal;

Id IdParser::createId()
{
	return Id("qrm:");
}

Id IdParser::append(Id const &source, QString part)
{
	Id result = source;
	result.setPath(result.path() + "/" + part);
	Q_ASSERT(getIdSize(result) <= 4);
	return result;
}

QString IdParser::getEditor(Id const &id)
{
	return getPathElement(id, 1);
}

QString IdParser::getDiagram(Id const &id)
{
	return getPathElement(id, 2);
}

QString IdParser::getElement(Id const &id)
{
	return getPathElement(id, 3);
}

QString IdParser::getId(Id const &id)
{
	return getPathElement(id, 4);
}

unsigned IdParser::getIdSize(Id const &id) {
	unsigned pathSize = getPath(id).size();
	Q_ASSERT(pathSize >= 1);
	return pathSize - 1;
}

QStringList IdParser::getPath(Id const &id)
{
	Q_ASSERT(id.scheme() == "qrm");
	return id.path().split('/');
}

QString IdParser::getPathElement(Id const & id, int const &index)
{
	QStringList path = getPath(id);
	Q_ASSERT(path.size() >= index + 1);
	return path[index];
}
