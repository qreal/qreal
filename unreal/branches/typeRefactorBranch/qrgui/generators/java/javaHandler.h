#pragma once

#include <QtCore/QString>

#include "../../kernel/NewType.h"

namespace qrRepo {
    class RepoApi;
}

namespace qReal {

    namespace generators {

        class JavaHandler
        {
        public:
            explicit JavaHandler(qrRepo::RepoApi const &api);

            QString generateToJava(QString const &pathToDir);
            QString parseJavaLibraries(QString const &pathToDir);
        private:
            bool checkTheModel();
            bool commentAndConstraintChecking(TypeList const &idList, QString const &diagramType, QString const &nodeType);

            TypeList getActivityChildren(NewType const &idStartNode, NewType const &untilNode);

            QString serializeObject(NewType const &type);
            QString serializeChildren(NewType const &type);
            QString serializeActivity(NewType const &idStartNode, NewType const &idUntilNode);

            QString getVisibility(NewType const &type);
            QString getMultiplicity(NewType const &type);
            QString getType(NewType const &type);
            QString getDefaultValue(NewType const &type);
            QString getImports(NewType const &type);
            QString getConstraints(NewType const &type);
            QString getConstraint(NewType const &type);
            QString getComments(NewType const &type);
            QString getComment(NewType const &type);
            QString getOperationFactors(NewType const &type);
            QString hasModifier(NewType const &type, QString const &modifier);
            QString getSuperclass(NewType const &type);
            QString getInterfaces(NewType const &type);
            QString getMethodCode(NewType const &type);
            QString getFlowGuard(NewType const &type);
            QString serializeMultiplicity(NewType const &type, QString const &multiplicity) const;

            bool isTypeSuitable(QString const &type) const;
            bool isVisibilitySuitable(QString const &type) const;

            QString objectType(NewType const &type);
            QString tryCatch(NewType const &type);
            QString ifStatement(NewType const &type);
            QString whileDoLoop(NewType const &type);

            NewType findMergeNode(NewType const &idDecisionNode);
            NewType findNonBodyLink(NewType const &idDecisionNode);
            TypeList findIntermediateNodes(NewType const &type, NewType const &untilNode, bool const closesFunction);
            TypeList deleteCommentLinks(TypeList &idList);
            TypeList deleteConstraintEdges(TypeList &idList);

            void addError(QString const &errorText);

            qrRepo::RepoApi const &mApi;
            QString mErrorText;
            QString pathToDir;

            int mIndent;
            QString indent();

            //Parsing Java Libraries
            QStringList getAllFilesInDirectory(QString dir_name);
        };

    }
}
