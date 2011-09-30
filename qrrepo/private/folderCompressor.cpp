#include "folderCompressor.h"

FolderCompressor::FolderCompressor()
{
}

bool FolderCompressor::compressFolder(QString sourceFolder, QString destinationFile) {
    QDir src(sourceFolder);
	if(!src.exists()) { //folder not found
        return false;
    }

    file.setFileName(destinationFile);
	if(!file.open(QIODevice::WriteOnly)) { //could not open file
        return false;
    }

    dataStream.setDevice(&file);

    bool success = compress(sourceFolder, "");
    file.close();

    return success;
}

bool FolderCompressor::compress(QString sourceFolder, QString prefex) {
    QDir dir(sourceFolder);
    if(!dir.exists())
        return false;

    //1 - list all folders inside the current folder
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    QFileInfoList foldersList = dir.entryInfoList();

    //2 - For each folder in list: call the same function with folders' paths
    for(int i=0; i<foldersList.length(); i++)
    {
        QString folderName = foldersList.at(i).fileName();
        QString folderPath = dir.absolutePath()+"/"+folderName;
        QString newPrefex = prefex+"/"+folderName;

        compress(folderPath, newPrefex);
    }

    //3 - List all files inside the current folder
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    QFileInfoList filesList = dir.entryInfoList();

    //4- For each file in list: add file path and compressed binary data
    for(int i=0; i<filesList.length(); i++)
    {
        QFile file(dir.absolutePath()+"/"+filesList.at(i).fileName());
        if(!file.open(QIODevice::ReadOnly))//couldn't open file
        {
            return false;
        }

        dataStream << QString(prefex+"/"+filesList.at(i).fileName());
        dataStream << qCompress(file.readAll());

		file.close();
	}

    return true;
}

bool FolderCompressor::decompressFolder(QString sourceFile, QString destinationFolder)
{
    //validation
    QFile src(sourceFile);
    if(!src.exists())
    {//file not found, to handle later
        return false;
    }
    QDir dir;
    if(!dir.mkpath(destinationFolder))
    {//could not create folder
        return false;
    }

    file.setFileName(sourceFile);
    if(!file.open(QIODevice::ReadOnly))
        return false;

    dataStream.setDevice(&file);

    while(!dataStream.atEnd())
    {
        QString fileName;
        QByteArray data;

        //extract file name and data in order
        dataStream >> fileName >> data;

        //create any needed folder
        QString subfolder;
        for(int i=fileName.length()-1; i>0; i--)
        {
            if((QString(fileName.at(i)) == QString("\\")) || (QString(fileName.at(i)) == QString("/")))
            {
                subfolder = fileName.left(i);
                dir.mkpath(destinationFolder+"/"+subfolder);
                break;
            }
        }

        QFile outFile(destinationFolder+"/"+fileName);
        if(!outFile.open(QIODevice::WriteOnly))
        {
            file.close();
            return false;
        }
        outFile.write(qUncompress(data));
        outFile.close();
    }

    file.close();
    return true;
}
