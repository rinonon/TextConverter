#include "filemanager.h"
#include <QDebug>
#include <QString>
#include <QChar>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVector>


QString FileManager::convertToJson(QString body)
{
    //キーのまとまりでわける
    QStringList scenes = body.split(_keySplitStr);
    QJsonObject data;

    for(int i = 0; i < scenes.size(); i++)
    {
        QJsonObject keys;
        QString keyStr = "";

        //段落のまとまりでわける
        QStringList paragraphs = scenes.at(i).split(_objectSplitStr);

        for(int j = 0; j < paragraphs.size(); j++)
        {
            QString str = paragraphs.at(j);

            if(j == 0)
            {
                //最初のみキーを取り出す
                int index = str.indexOf('\n');
                for(int k = 0; k < index; k++)
                {
                    keyStr += str.at(k);
                }
                str = str.remove(0, index + 1);
            }

            QJsonObject tutorial;
            tutorial["message"] = str;
            keys.insert(QString("paragraph%1").arg(j), tutorial);
        }
        data.insert(keyStr, keys);
    }

    QJsonDocument doc(data);

    //書き出し形式指定して文字列に
    QString ret = doc.toJson(QJsonDocument::Compact);
    //QString ret = doc.toJson(QJsonDocument::Indented);

    return ret;
}


QString FileManager::convertToCSV(QString body)
{
    //まずキーのまとまりでわける
    QStringList scenes = body.split(_keySplitStr);

    //縦長（イメージ）の文字列配列をつくる
    std::vector<std::vector<QString>> keys(scenes.size(), std::vector<QString>());

    int maxLength = 0; //最大の長さをとっておく

    for(int i = 0; i < scenes.size(); i++)
    {
        int length = 0;

        //段落のまとまりでわける
        QStringList paragraphs = scenes.at(i).split(_objectSplitStr);

        for(int j = 0; j < paragraphs.size(); j++)
        {
            QString str = paragraphs.at(j);

            if(j == 0)
            {
                //最初のみキーを取り出す
                QString keyStr = "";
                int index = str.indexOf('\n');
                for(int k = 0; k < index; k++)
                {
                    keyStr += str.at(k);
                }

                keys.at(i).push_back(keyStr);       //キーを格納
                str = str.remove(0, index + 1);     //キーを削除した値にする
                length++;
            }

            str.replace('\n', _lineFeedCode);
            keys.at(i).push_back(str);
            length++;
        }
        maxLength = std::max(length, maxLength);
        qDebug() << maxLength;
    }

    QString ret = "";

   for(int i = 0; i < maxLength; i++)
   {
       for(int j = 0; j < static_cast<int>(keys.size()); j++)
       {
           if(static_cast<int>(keys.at(j).size()) <= i)
           {
               ret += "";   //中身が無いので空文字
           }
           else
           {
               ret += keys.at(j).at(i);
           }

           if(j != static_cast<int>(keys.size()) - 1)
           {
               ret += ',';
           }
       }
       ret += '\n';
    }

    return ret;
}


void FileManager::saveFile(QString fileName, QString body)
{
    QString dirStr = FileManager::getInstance()->getSaveDirectoryPath();

    //セーブファイルを格納するディレクトリを作る
    QDir saveDir;
    if(!saveDir.exists(dirStr))
    {
        saveDir.mkpath(dirStr);
        qDebug() << "Created";
    }
    else
    {
        qDebug() << "Already Exists";
    }

    //書き込む
    QFile file(dirStr + "/" + fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        //オープンできないので終わる
        return;
    }

    QTextStream out(&file);
    out << body;
}


QString FileManager::loadFile(QString fileName)
{
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
    {
        //オープンできないので終わる
        return "";
    }

    QTextStream in(&file);
    QString ret = "";
    in >> ret;;

    return ret;
}


QString FileManager::getSaveDirectoryPath()
{
    //カレントディレクトリ取得
    QDir curDir(QDir::current());
    curDir.cd("../../../");
    QString dirStr(curDir.canonicalPath());
    dirStr += "/SaveFile";

    return dirStr;
}
