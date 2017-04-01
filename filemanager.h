#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "singleton.h"
#include <QString>

class FileManager : public Singleton<FileManager>
{
public:
    FileManager()
        : _lineFeedCode("\n")
        , _keySplitStr("\n\n\n")
        , _objectSplitStr("\n\n")
    {};

    virtual ~FileManager()
    {};

    QString convertToJson(QString body);
    QString convertToCSV(QString body);
    void saveFile(QString fileName, QString body);
    void setLineFeedCode(QString code){ _lineFeedCode = code; };
    QString loadFile(QString fileName);
    QString getSaveDirectoryPath();
    QString getKeySplitStr(){ return _keySplitStr; };
    QString getObjectSplitStr(){ return _objectSplitStr; };

private:
    QString _lineFeedCode;
    QString _keySplitStr;
    QString _objectSplitStr;
};

#endif // FILEMANAGER_H
