#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:


private slots:
    void on_buttonOpen_clicked();
    void on_buttonGenerate_clicked();
    void on_buttonExit_clicked();
    void on_buttonSave_clicked();
    void on_buttonGenerateHtml_clicked();
};

#endif // MAINWINDOW_H
