#include "javaLexer.h"
#include "javaParser.h"
#include <activityLexer.h>
#include <activityParser.h>
#include "structure.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>

#include <QFileInfo>
#include <QDir>

#include <QtCore/QDebug>

static pjavaLexer lxr;
static pactivityLexer activityLxr;


QStringList getJavaFiles(QString dir_name);
static javaParser_compilationUnit_return parseFile(pANTLR3_UINT8 fileName);
static pANTLR3_STRING className(pANTLR3_BASE_TREE tree);
static pANTLR3_STRING classAttributesInString(pANTLR3_BASE_TREE tree);
static QStringList classAttributes(pANTLR3_BASE_TREE tree);
static pANTLR3_STRING treeToString(pANTLR3_BASE_TREE tree);
static activityParser_compilationUnit_return parseBlock(pANTLR3_UINT8 blockStatement);

int main (int argc, char *argv[])
{
    qDebug() << "start";

    QString const pathToFile = "D:/LeftDocs/mini/BlockStatementFile";
    QFile file(pathToFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return 1;
    QTextStream out(&file);

    QString blockStm = "int i = 0;\n";
    out << blockStm;

    QByteArray byteArray = pathToFile.toLatin1();
    char * blockStmFile = byteArray.data();
    activityParser_compilationUnit_return blockStatement = parseBlock((pANTLR3_UINT8) blockStmFile);
    pANTLR3_BASE_TREE blockTree = blockStatement.tree;
    pANTLR3_STRING blockTreeString = blockTree->toStringTree(blockTree);
    qDebug() << "blockTree = " + QString(QLatin1String((char*) blockTreeString->chars));

//    QString path = "D:/LeftDocs/mini/jdk";
////    QString directory = path + "/jdkFiles";
////    QString filePath = path + "/scr.zip";
////    bool result = testRead(directory, filePath);
////    QStringList files = getJavaFiles(directory);
//    QStringList files = getJavaFiles(path);
//    foreach (QString aFile, files) {
////    QString aFile = files.at(1);
//        qDebug() << aFile;
//
//        //QString -> char *
//        QByteArray byteArray = aFile.toLatin1();
//        char * fileName = byteArray.data();
//
//        javaParser_compilationUnit_return compilationUnit = parseFile((pANTLR3_UINT8)fileName);
//        pANTLR3_BASE_TREE tree = compilationUnit.tree;
//
//        structure fileStructure;
//
//        pANTLR3_STRING name = className(tree);
//        qDebug() << "className = " + QString(QLatin1String((char*)name->chars));
//
//        QStringList attributes = classAttributes(tree);
//        foreach (QString anAttr, attributes) {
//            qDebug() << "attribute = " + anAttr;
//        }
//    }

    qDebug() << "finished parsing OK";

    return 0;
}

QStringList getJavaFiles(QString dir_name)
{
    QStringList ret_list;
    QDir dir(dir_name);
    QFileInfoList info_list = dir.entryInfoList();
    if(info_list.size() > 2) {
        QList<QFileInfo>::iterator iter = info_list.begin();
        QString path;
        for (iter = info_list.begin() + 2; iter != info_list.end(); iter++) {
            path = iter->absoluteFilePath();
            if (iter->isDir()) {
                ret_list += getJavaFiles(path);
            } else {
                if (path.endsWith(".java")) {
                    qDebug() << "adding file with path = " + path;
                    ret_list.append(path);
                }
            }
        }
    }
    return ret_list;
}

static javaParser_compilationUnit_return parseFile(pANTLR3_UINT8 fileName)
{
    javaParser_compilationUnit_return result;

    // Now we declare the ANTLR related local variables we need.
    // Note that unless you are convinced you will never need thread safe
    // versions for your project, then you should always create such things
    // as instance variables for each invocation.
    // -------------------

    // The ANTLR3 character input stream, which abstracts the input source such that
    // it is easy to provide input from different sources such as files, or
    // memory strings.
    //
    // For an ASCII/latin-1 memory string use:
    //	    input = antlr3NewAsciiStringInPlaceStream (stringtouse, (ANTLR3_UINT64) length, NULL);
    //
    // For a UCS2 (16 bit) memory string use:
    //	    input = antlr3NewUCS2StringInPlaceStream (stringtouse, (ANTLR3_UINT64) length, NULL);
    //
    // For input from a file, see code below
    //
    // Note that this is essentially a pointer to a structure containing pointers to functions.
    // You can create your own input stream type (copy one of the existing ones) and override any
    // individual function by installing your own pointer after you have created the standard
    // version.
    //
    pANTLR3_INPUT_STREAM input;


    // The token stream is produced by the ANTLR3 generated lexer. Again it is a structure based
    // API/Object, which you can customise and override methods of as you wish. a Token stream is
    // supplied to the generated parser, and you can write your own token stream and pass this in
    // if you wish.
    //
    pANTLR3_COMMON_TOKEN_STREAM tstream;

    // The C parser is also generated by ANTLR and accepts a token stream as explained
    // above. The token stream can be any source in fact, so long as it implements the
    // ANTLR3_TOKEN_SOURCE interface. In this case the parser does not return anything
    // but it can of course specify any kind of return type from the rule you invoke
    // when calling it.
    //
    pjavaParser	psr;

    // Create the input stream using the supplied file name
    // (Use antlr3AsciiFileStreamNew for UCS2/16bit input).
    //
    input = antlr3AsciiFileStreamNew(fileName);

    // The input will be created successfully, providing that there is enough
    // memory and the file exists etc
    //
    if ( input == NULL ) {
        qDebug() << "File not found: " + QString(QLatin1String((char *)fileName));
        exit(1);
    }

    // Our input stream is now open and all set to go, so we can create a new instance of our
    // lexer and set the lexer input to our input stream:
    //  (file | memory | ?) --> inputstream -> lexer --> tokenstream --> parser ( --> treeparser )?
    //
    if (lxr == NULL) {
        lxr = javaLexerNew(input);	    // javaLexerNew is generated by ANTLR
    } else {
        lxr->pLexer->setCharStream(lxr->pLexer, input);
    }

    // Need to check for errors
    //
    if (lxr == NULL) {
        qDebug() << "Unable to create the lexer due to malloc() failure1\n";
        exit(ANTLR3_ERR_NOMEM);
    }

    // Our lexer is in place, so we can create the token stream from it
    // NB: Nothing happens yet other than the file has been read. We are just
    // connecting all these things together and they will be invoked when we
    // call the parser rule. ANTLR3_SIZE_HINT can be left at the default usually
    // unless you have a very large token stream/input. Each generated lexer
    // provides a token source interface, which is the second argument to the
    // token stream creator.
    // Note that even if you implement your own token structure, it will always
    // contain a standard common token within it and this is the pointer that
    // you pass around to everything else. A common token as a pointer within
    // it that should point to your own outer token structure.
    //
    tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));

    if (tstream == NULL) {
        qDebug() << "Out of memory trying to allocate token stream\n";
        exit(ANTLR3_ERR_NOMEM);
    }

    // Finally, now that we have our lexer constructed, we can create the parser
    //
    psr = javaParserNew(tstream);  // javaParserNew is generated by ANTLR3

    if (tstream == NULL) {
        qDebug() << "Out of memory trying to allocate parser\n";
        exit(ANTLR3_ERR_NOMEM);
    }

    // We are all ready to go. Though that looked complicated at first glance,
    // I am sure, you will see that in fact most of the code above is dealing
    // with errors and there isn't really that much to do (isn't this always the
    // case in C? ;-).
    //
    // So, we now invoke the parser. All elements of ANTLR3 generated C components
    // as well as the ANTLR C runtime library itself are pseudo objects. This means
    // that they are represented as pointers to structures, which contain any
    // instance data they need, and a set of pointers to other interfaces or
    // 'methods'. Note that in general, these few pointers we have created here are
    // the only things you will ever explicitly free() as everything else is created
    // via factories, that allocate memory efficiently and free() everything they use
    // automatically when you close the parser/lexer/etc.
    //
    // Note that this means only that the methods are always called via the object
    // pointer and the first argument to any method, is a pointer to the structure itself.
    // It also has the side advantage, if you are using an IDE such as VS2005 that can do it,
    // that when you type ->, you will see a list of all the methods the object supports.
    //
    //putc('L', stdout); fflush(stdout);
        //{
        //	ANTLR3_INT32 T;

        //	T = 0;
        //	while	(T != ANTLR3_TOKEN_EOF)
        //	{
        //		T = tstream->tstream->istream->_LA(tstream->tstream->istream, 1);
        //		tstream->tstream->istream->consume(tstream->tstream->istream);
        //		printf("%d %s\n", T,  (psr->pParser->rec->state->tokenNames)[T]);
        //	}
        //}
    tstream->tstream->_LT(tstream->tstream, 1);	// Don't do this normally, just causes lexer to run for timings here
    //putc('P', stdout); fflush(stdout);

    result = psr->compilationUnit(psr);

    putc('*', stdout);
    fflush(stdout);

    // We did not return anything from this parser rule, so we can finish. It only remains
    // to close down our open objects, in the reverse order we created them
    //
//    psr->free(psr);
//    psr = NULL;
//    tstream->free(tstream);
//    tstream = NULL;
//    lxr->free(lxr);
//    lxr = NULL;
//    input->close(input);
//    input = NULL;

    return result;
}

static pANTLR3_STRING className(pANTLR3_BASE_TREE tree)
{
    pANTLR3_STRING  string;
    ANTLR3_UINT32   i;
    ANTLR3_UINT32   n;
    pANTLR3_BASE_TREE   t;
    bool shouldAppend = false;

    if	(tree->children == NULL || tree->children->size(tree->children) == 0)
    {
            return	className(tree);
    }

    /* Need a new string with nothing at all in it.
    */
    string = tree->strFactory->newRaw(tree->strFactory);

    if	(tree->children != NULL)
    {
            n = tree->children->size(tree->children);

            for	(i = 0; i < n; i++)
            {
                    t   = (pANTLR3_BASE_TREE) tree->children->get(tree->children, i);

                    pANTLR3_STRING toString = t->toString(t);
                    if (shouldAppend) {
                        string->appendS(string, toString);
                        return string;
                    }
                    if ( QString(QLatin1String((char*)(toString->chars))) == "class"
                         || QString(QLatin1String((char*)(toString->chars))) == "interface" ) {
                        string->appendS(string, toString);
                        string->append8(string, " ");
                        shouldAppend = true;
                    }
            }
    }

    return  string;
}

static pANTLR3_STRING classAttributesInString(pANTLR3_BASE_TREE tree)
{
    pANTLR3_STRING  string;
    ANTLR3_UINT32   i;
    ANTLR3_UINT32   n;
    pANTLR3_BASE_TREE   t;
    bool shouldAppend = false;

    if	(tree->children == NULL || tree->children->size(tree->children) == 0)
    {
            return	classAttributesInString(tree);
    }

    /* Need a new string with nothing at all in it.
    */
    string = tree->strFactory->newRaw(tree->strFactory);

    if	(tree->children != NULL)
    {
            n = tree->children->size(tree->children);

            for	(i = 0; i < n; i++)
            {
                    t   = (pANTLR3_BASE_TREE) tree->children->get(tree->children, i);

                    if  (i > 0)
                    {
                            string->append8(string, " ");
                    }
                    pANTLR3_STRING toString = t->toString(t);
                    if (shouldAppend) {
                        if (QString(QLatin1String((char *)toString->chars)) != "{"){
                            string->appendS(string, toString);
                        } else {
                            string->append8(string, ";");
                        }
                        string->append8(string, " ");
                    }
                    if (QString(QLatin1String((char *)toString->chars)) == "public" || QString(QLatin1String((char *)toString->chars)) == "protected") {
                        string->appendS(string, toString);
                        string->append8(string, " ");
                        shouldAppend = true;
                    }
                    if (QString(QLatin1String((char *)toString->chars)) == ";" || QString(QLatin1String((char *)toString->chars)) == "{") {
                        shouldAppend = false;
                    }
            }
    }

    return  string;
}

static QStringList classAttributes(pANTLR3_BASE_TREE tree)
{
    QStringList result;

    pANTLR3_STRING attributesANTLR3 = classAttributesInString(tree);
    QString attributes = QString(QLatin1String((char *) attributesANTLR3 -> chars));
    attributes = attributes.simplified();
    result = attributes.split(";");

    QString last = result.takeLast();
    if (last != "") {
        result.append(last);
    }

    return result;
}

static pANTLR3_STRING treeToString(pANTLR3_BASE_TREE tree)
{
    pANTLR3_STRING  string;
    ANTLR3_UINT32   i;
    ANTLR3_UINT32   n;
    pANTLR3_BASE_TREE   t;

//    qDebug() << " [type = " + QString((int) tree->getType(tree)) + "] ";

    if	(tree->children == NULL || tree->children->size(tree->children) == 0)
    {
        return	tree->toString(tree);
    }

    /* Need a new string with nothing at all in it.
    */
    string	= tree->strFactory->newRaw(tree->strFactory);

    if	(tree->isNilNode(tree) == ANTLR3_FALSE)
    {
        string->append8	(string, "(");
        string->appendS	(string, tree->toString(tree));
        string->append8	(string, " ");
    }
    if	(tree->children != NULL)
    {
        n = tree->children->size(tree->children);

        for	(i = 0; i < n; i++)
        {
            t   = (pANTLR3_BASE_TREE) tree->children->get(tree->children, i);

            if  (i > 0)
            {
                string->append8(string, " ");
            }
            string->appendS(string, treeToString(t));
        }
    }
    if	(tree->isNilNode(tree) == ANTLR3_FALSE)
    {
        string->append8(string,")");
    }

    return  string;
}

static activityParser_compilationUnit_return parseBlock(pANTLR3_UINT8 blockStatementFile)
{
    activityParser_compilationUnit_return result;

    pANTLR3_INPUT_STREAM input;

    pANTLR3_COMMON_TOKEN_STREAM tstream;

    pactivityParser	psr;

    input = antlr3AsciiFileStreamNew(blockStatementFile);

    if ( input == NULL ) {
        qDebug() << "File not found: " + QString(QLatin1String((char *)blockStatementFile));
        exit(1);
    }

    if (activityLxr == NULL) {
        activityLxr = activityLexerNew(input);
    } else {
        activityLxr->pLexer->setCharStream(activityLxr->pLexer, input);
    }

    if (activityLxr == NULL) {
        qDebug() << "Unable to create the lexer due to malloc() failure1\n";
        exit(ANTLR3_ERR_NOMEM);
    }

    tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(activityLxr));

    if (tstream == NULL) {
        qDebug() << "Out of memory trying to allocate token stream\n";
        exit(ANTLR3_ERR_NOMEM);
    }

    psr = activityParserNew(tstream);

    if (tstream == NULL) {
        qDebug() << "Out of memory trying to allocate parser\n";
        exit(ANTLR3_ERR_NOMEM);
    }

    tstream->tstream->_LT(tstream->tstream, 1);	// Don't do this normally, just causes lexer to run for timings here
    //putc('P', stdout); fflush(stdout);

    result = psr->compilationUnit(psr);

    putc('*', stdout);
    fflush(stdout);

    return result;
}
