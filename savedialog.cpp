#include "savedialog.h"
#include "ui_savedialog.h"

SaveDialog::SaveDialog(QWidget *parent,
                       FileType type,
                       Callback callback) :
    QDialog(parent),
    ui(new Ui::SaveDialog)
{
    ui->setupUi(this);
    _callback = callback;
    _type = type;
}

SaveDialog::~SaveDialog()
{
    delete ui;
}

void SaveDialog::on_saveButton_clicked()
{
    //ファイル名を取得してメインウィンドウに渡す
    QString fileName = ui->lineEdit->text();

    //拡張子
    QString extensionStr = "";
    switch(_type)
    {
    case FileType::String:
    {
        extensionStr = ".txt";
        break;
    }
    case FileType::Json:
    {
        extensionStr = ".json";
        break;
    }
    case FileType::CSV:
    {
        extensionStr = ".csv";
        break;
    }
    default:
        break;
    }

    fileName += extensionStr;

    _callback(true, fileName);
    this->close();
}

void SaveDialog::on_cancelButton_clicked()
{
    this->close();
}
