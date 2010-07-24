#include "edgeType.h"
#include "../utils/outFile.h"
#include "metaCompiler.h"
#include "diagram.h"
#include "editor.h"
#include "utils/nameNormalizer.h"

#include <QDebug>

using namespace utils;

EdgeType::EdgeType(Diagram *diagram, qrRepo::RepoApi *api, const qReal::Id &id) : GraphicType(diagram, api, id)
{}

EdgeType::~EdgeType()
{
}

Type* EdgeType::clone() const
{
	EdgeType *result = new EdgeType(mDiagram, mApi, mId);
	GraphicType::copyFields(result);
	result->mBeginType = mBeginType;
	result->mEndType = mEndType;
	result->mLineType = mLineType;
	return result;
}

void EdgeType::print()
{
	qDebug() << "edge" << mName;
}
