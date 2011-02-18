#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QMetaType>

namespace qReal {

        class NewType {
        public:
                static NewType loadFromString(QString const &string);
                static NewType rootType();

                explicit NewType(QString const &editor = "", QString  const &diagram = "",
                        QString  const &element = "");
                NewType(NewType const &base, QString const &additional);

                NewType rootId() const;
                NewType editorId() const;
                NewType diagramId() const;

                QUrl toUrl() const;
                QString id() const;
                QString editor() const;
                QString diagram() const;
                QString element() const;
                QString toString() const;
                unsigned typeSize() const;

                // cast to QVariant. not an operator, to avoid problems with autocasts
                QVariant toVariant() const;

                // default destructor and copy constuctor are OK
        private:
                QString mEditor;
                QString mDiagram;
                QString mElement;
                QString mId;

                // used only for debug
                bool checkIntegrity() const;
        };

        inline bool operator==(const NewType &i1, const NewType &i2)
        {
                return i1.editor() == i2.editor()
                        && i1.diagram() == i2.diagram()
                        && i1.element() == i2.element();
        }

        inline bool operator!=(const NewType &i1, const NewType &i2)
        {
                return !(i1 == i2);
        }

        inline uint qHash(const NewType &key)
        {
                return qHash(key.editor()) ^ qHash(key.diagram()) ^ qHash(key.element());
        }

        typedef QList<NewType> TypeList;

        class TypeListHelper {
        public:
                static QVariant toVariant(TypeList const &list);
        };
}

// qReal::Id and qReal::IdList could be used straight in QVariant
Q_DECLARE_METATYPE(qReal::NewType)

Q_DECLARE_METATYPE(qReal::TypeList)
