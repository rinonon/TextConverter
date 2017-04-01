#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    enum class FileType
    {
        None = -1,

        String,
        Json,
        CSV,
    };

    typedef std::function<void(bool, QString)> Callback;

    explicit SaveDialog(QWidget *parent = 0,
                        FileType type = FileType::None,
                        Callback callback = nullptr);
    ~SaveDialog();

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::SaveDialog *ui;
    Callback _callback;
    FileType _type;
};

#endif // SAVEDIALOG_H
