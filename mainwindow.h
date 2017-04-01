#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "savedialog.h"

namespace Ui {
class MainWindow;
}

class SaveDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
  void on_lineFeedButton_clicked();
  void on_JsonButton_clicked();
  void on_CSVButton_clicked();
  void on_actionSave_triggered();
  void on_plainTextEdit_textChanged();
  void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    SaveDialog *savedialog;

    bool check();
    void insertLineFeed();
    void createSaveDialog(SaveDialog::FileType type);
};

#endif // MAINWINDOW_H
