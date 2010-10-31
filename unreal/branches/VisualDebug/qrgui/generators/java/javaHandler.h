#pragma once

#include <QtCore/QString>

#include "../../kernel/ids.h"

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
            bool commentAndConstraintChecking(IdList const &idList, QString const &diagramType, QString const &nodeType);

            IdList getActivityChildren(Id const &idStartNode, Id const &untilNode);

            QString serializeObject(Id const &id);
            QString serializeChildren(Id const &id);
            QString serializeActivity(Id const &idStartNode, Id const &idUntilNode);

            QString getVisibility(Id const &id);
            QString getMultiplicity(Id const &id);
            QString getType(Id const &id);
            QString getDefaultValue(Id const &id);
            QString getImports(Id const &id);
            QString getConstraints(Id const &id);
            QString getConstraint(Id const &id);
            QString getComments(Id const &id);
            QString getComment(Id const &id);
            QString getOperationFactors(Id const &id);
            QString hasModifier(Id const &id, QString const &modifier);
            QString getSuperclass(Id const &id);
            QString getInterfaces(Id const &id);
            QString getMethodCode(Id const &id);
            QString getFlowGuard(Id const &id);
            QString serializeMultiplicity(Id const &id, QString const &multiplicity) const;

            bool isTypeSuitable(QString const &type) const;
            bool isVisibilitySuitable(QString const &type) const;

            QString objectType(Id const &id);
            QString tryCatch(Id const &id);
            QString ifStatement(Id const &id);
            QString whileDoLoop(Id const &id);

            Id findMergeNode(Id const &idDecisionNode);
            Id findNonBodyLink(Id const &idDecisionNode);
            IdList findIntermediateNodes(Id const &id, Id const &untilNode, bool const closesFunction);
            IdList deleteCommentLinks(IdList &idList);
            IdList deleteConstraintEdges(IdList &idList);

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
