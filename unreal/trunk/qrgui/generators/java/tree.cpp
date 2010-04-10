#include "tree.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/definitions.h"
#include "../../../qrrepo/repoApi.h"

using namespace qReal;
using namespace generators;

Tree::Tree(Id const &element, TreeList const &children)
    : mElement(element), mChildren(children)
{
}

Id Tree::element()
{
    return mElement;
}

QList<Tree> Tree::children()
{
    return mChildren;
}

void Tree::moveChildToEnd(Tree const &child)
{
    int index = mChildren.indexOf(child);
    mChildren.swap(index, mChildren.length()-1);
}
