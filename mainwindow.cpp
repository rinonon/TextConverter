#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "savedialog.h"
#include "filemanager.h"
#include <QDebug>
#include <QString>
#include <QChar>
#include <QPoint>
#include <QCursor>
#include <QTextBlock>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::check()
{
    QString body = ui->plainTextEdit->toPlainText();
    int maxCount = (ui->maxCount->text()).toInt();

    //文字数を超えていないかチェック
    QString errorStr ="";
    bool ret = true;

    //キーのまとまりでわける
    QStringList keys = body.split(FileManager::getInstance()->getKeySplitStr());

    for(int i = 0; i < keys.size(); i++)    //キーごとのるーぷ（改行3個分）
    {
        //段落のまとまりで分ける
        QString key = keys.at(i);
        QStringList paragraphs = key.split(FileManager::getInstance()->getObjectSplitStr());
        QString currentKey = "";
        int line = 0;

        for(int j = 0; j < paragraphs.size(); j++)  //段落ごとのループ（改行2個分）
        {
            //文のまとまりごとで分ける
            QString paragraph = paragraphs.at(j);
            QStringList sentences = paragraph.split('\n');

            for(int k = 0; k < sentences.size(); k++)
            {
                QString sentence = sentences.at(k);

                //キーなら判定しない
                if(k == 0 && j == 0)
                {
                    currentKey = sentence;
                    continue;
                }

                //文字数オーバーしてないかチェック
                if(sentence.size() > maxCount)
                {
                    errorStr += QString("key:%1 paragraph:%2 limit over\n").arg(currentKey).arg(j + 1);
                    ret = false;
                }
            }
        }
    }
    ui->textEdit->setPlainText(errorStr);

    return ret;
}

void MainWindow::insertLineFeed()
{
    int maxCount = ui->maxCount->text().toInt();
    if(maxCount <= 0)
    {
        return;
    }

    QString body = ui->plainTextEdit->toPlainText();
    QString converted = "";     //変換結果を格納

    //キーのまとまりでわける
    QStringList keys = body.split(FileManager::getInstance()->getKeySplitStr());

    for(int i = 0; i < keys.size(); i++)
    {
        QString myString = keys.at(i);  //書き換えるために新たにインスタンス作る
        myString.append(FileManager::getInstance()->getKeySplitStr());

        //keyは自動改行対象外
        int keyIdx = keys.at(i).indexOf('\n') + 1;
        int count = 0;

        for(int j = keyIdx; j < myString.size(); j++)
        {
            QChar c = myString.at(j);
            count++;

            if(c == '\n')
            {
                count = 0;
            }

            if(count > maxCount)
            {
                myString.insert(j, "\n");
                count = 0;
                qDebug() << "insert";
            }
        }
        converted += myString;
    }
    ui->plainTextEdit->setPlainText(converted);
}


void MainWindow::createSaveDialog(SaveDialog::FileType type)
{
    savedialog = new SaveDialog(this, type,
                                [this, type](bool ret, QString fileName)
    {
        if(ret)
        {
            FileManager* fm = FileManager::getInstance();
            QString body = ui->plainTextEdit->toPlainText();
            QString lineFeedCode = ui->lineFeedCode->text();

            //ファイルマネージャーに改行コード設定
            fm->setLineFeedCode(lineFeedCode);

            switch(type)
            {
            case SaveDialog::FileType::String:
            {
                fm->saveFile(fileName, body);
                break;
            }
            case SaveDialog::FileType::Json:
            {
                QString json = fm->convertToJson(body);
                fm->saveFile(fileName, json);
                break;
            }
            case SaveDialog::FileType::CSV:
            {
                QString csv = fm->convertToCSV(body);
                fm->saveFile(fileName, csv);
                break;
            }
            default:
                break;
            }
        }
    });

    savedialog->setWindowTitle("SaveDialog");
    savedialog->show();
}


// ---以下スロット関数---
void MainWindow::on_lineFeedButton_clicked()
{
    int pos = ui->plainTextEdit->textCursor().blockNumber();
    this->insertLineFeed();

    QTextBlock block = ui->plainTextEdit->document()->findBlockByLineNumber(pos + 1);
    QTextCursor cursor = QTextCursor(block);
    ui->plainTextEdit->setTextCursor(cursor);
}

void MainWindow::on_JsonButton_clicked()
{
    this->createSaveDialog(SaveDialog::FileType::Json);
}

void MainWindow::on_CSVButton_clicked()
{
    this->createSaveDialog(SaveDialog::FileType::CSV);
}

void MainWindow::on_plainTextEdit_textChanged()
{
    //テキストが変わるたび都度チェック走らせる
    this->check();
}

void MainWindow::on_actionLoad_triggered()
{
    QString saveDirPath = FileManager::getInstance()->getSaveDirectoryPath();

    QString fileName = QFileDialog::getOpenFileName(this, "Open File", saveDirPath, "*.txt");
    qDebug() << fileName;

    QString body = FileManager::getInstance()->loadFile(fileName);
    ui->plainTextEdit->setPlainText(body);
}

void MainWindow::on_actionSave_triggered()
{
    this->createSaveDialog(SaveDialog::FileType::String);
}

