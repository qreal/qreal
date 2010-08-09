#include "embeddedLinker.h"
#include "uml_nodeelement.h"

#include <QtGui/QStyle>
#include <QtGui/QGraphicsItem>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtCore/QDebug>

#include "../view/editorviewscene.h"

using namespace UML;
using namespace qReal;

EmbeddedLinker::EmbeddedLinker()
{
    mRectangle = QRectF(-6,-6,12,12);
    mInnerRectangle = QRectF(-3,-3,6,6);
    setAcceptsHoverEvents(true);
    QObject::connect(this,SIGNAL(coveredChanged()),this,SLOT(changeShowState()));
}

EmbeddedLinker::~EmbeddedLinker()
{
}

void EmbeddedLinker::setMaster(NodeElement *element)
{
    master = element;
    setParentItem(element);
    QObject::connect(master->scene(),SIGNAL(selectionChanged()),this,SLOT(changeShowState()));
}

void EmbeddedLinker::setCovered(bool arg)
{
    covered = arg;
    Q_EMIT coveredChanged();
    qDebug() << "covered == " << covered;
}

void EmbeddedLinker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
    painter->save();

    QBrush brush;
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->setOpacity(0.5);
    painter->setPen(Qt::blue);

    painter->drawEllipse(mRectangle);
    painter->setOpacity(0.7);
    painter->drawEllipse(mInnerRectangle);

    painter->restore();
//TODO:	сделать подсвечивание при наведении мыши
}

void EmbeddedLinker::moveTo(QPointF pos)
{
    QRectF bounding = master->boundingRect();

    float px = pos.x();
    float py = pos.y();
    float top = bounding.topLeft().y();
    float left = bounding.topLeft().x();
    float right = bounding.bottomRight().x();
    float bottom = bounding.bottomRight().y();

    float min = py - top;
    if (min > bottom - py)
        min = bottom - py;
    if (min > px - left)
        min = px - left;
    if (min > right - px)
        min = right - px;

    float fx;
    float fy;
    //obviously, top != left != right != bottom
    if ((bottom - py == min) || (py - top == min))
    {
        fx = px;
        if (bottom - py == min)
            fy = bottom;
        else
            fy = top;
    }
    else
    {
        fy = py;
        if (right - px == min)
            fx = right;
        else
            fx = left;
    }

    this->setPos(fx, fy);
}

QRectF EmbeddedLinker::boundingRect() const
{
    return mRectangle;
}

// Обработка событий мыши

void EmbeddedLinker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    EditorViewScene *scene = dynamic_cast<EditorViewScene*>(master->scene());

    if (scene != NULL)
    {
        const QString type = "qrm:/Kernel_metamodel/Kernel/Kernel_Association/";
        Id *edgeId = scene->createElement(type, event->scenePos());
        mEdge = dynamic_cast<EdgeElement*>(scene->getElem(*edgeId));
        if (mEdge != NULL)
        {
            mEdge->setSelected(true);
            master->setSelected(false);
            mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));	//QCursor?
        }
        else
            qDebug() << "*edge == NULL";
    }
}

void EmbeddedLinker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    mEdge->placeEndTo(mEdge->mapFromScene(mapToScene(event->pos())));
}

void EmbeddedLinker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    hide();
    EditorViewScene *scene = dynamic_cast<EditorViewScene*>(master->scene());
    if (scene != NULL)
    {
        mEdge->hide();
        NodeElement *under = dynamic_cast<NodeElement*>(scene->itemAt(event->scenePos()));
        mEdge->show();
        if (under == NULL)
            if (scene->launchEdgeMenu(mEdge, event->scenePos()))
                mEdge = NULL;
    }

    if (mEdge != NULL)
        mEdge->connectToPort();
}

void EmbeddedLinker::changeShowState()
{
    if ((master == NULL) || ((!master->scene()->selectedItems().contains(master)) &&
                    (!master->scene()->selectedItems().contains(mEdge))) || (!covered))
    {
        hide();
        return;
    }
    else if ((master->scene()->selectedItems().contains(master)) && covered)
        show();
}
