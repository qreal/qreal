#include "tree.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/definitions.h"
#include "../../../qrrepo/repoApi.h"

using namespace qReal;
using namespace generators;

Tree::Tree(Id const &element, QList<Tree> const &children)
    : mElement(element), mChildren(children)
{
}

bool Tree::operator== ( Tree const & other ) const
{
    if ( (mElement == other.element()) && (mChildren == other.children()) ) {
        return true;
    } else {
        return false;
    }
}

Id Tree::element() const
{
    return mElement;
}

QList<Tree> Tree::children() const
{
    return mChildren;
}

void Tree::moveChildToEnd(Tree const &child)
{
    int index = mChildren.indexOf(child);
    mChildren.swap(index, mChildren.length()-1);
}
