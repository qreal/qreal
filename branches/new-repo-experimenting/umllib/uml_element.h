#ifndef UML_ELEMENT_H
#define UML_ELEMENT_H

#include <QtGui/QGraphicsItem>
#include <QtCore/QModelIndex>

namespace UML {
	class Element : public QGraphicsItem
	{
	public:
		Element();

		QPersistentModelIndex index() const { return dataIndex; };
		void setIndex(QPersistentModelIndex &index);

		virtual void updateData();

		int uuid() const;    
		int type() const;
	protected:
		//       void mousePressEvent ( QGraphicsSceneMouseEvent * event );
		//       void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
		QPersistentModelIndex dataIndex;

		int m_uuid;
		int m_type;

		bool moving;
	};
};

#endif
