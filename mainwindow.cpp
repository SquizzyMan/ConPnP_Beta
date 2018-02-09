#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QTextEdit>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QXmlStreamWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//=============== Define variables ====================

QString inputFileName;
QFile inputFile(inputFileName);
QFile inputFileHtml(inputFileName);
QFile outputFile("out.txt");
QFile outputFileHtml("outhtml.txt");

//=====================================================
//=============== Open file dialog ====================
//=====================================================

void MainWindow::on_buttonOpen_clicked()
{
    inputFileName = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "Text File (*.txt) ;; HTML File (*.html)"

                );
    ui->lineEdit->setText(inputFileName);

}

//=====================================================
//================== Generator ========================
//=====================================================

void MainWindow::on_buttonGenerate_clicked()
{
    if (outputFile.open(QIODevice::WriteOnly))
    {
    QTextStream out (&outputFile);
    out << "Filename: \n\n" << "Position of PCB: \n" << "left under edge: X=0 / Y=0 \n\n" <<
           "name\t" << "X-axis\t" << "Y-axis\t" << "angle\t" << "value\t" << "package\t" << "side\n\n";
    out << "REF1T\t0.00\t0.00\t0.00\t1mm\tCircle\ttop\n" << "REF2T\t0.00\t0.00\t0.00\t1mm\tCircle\ttop\n"
        << "REF1B\t0.00\t0.00\t0.00\t1mm\tCircle\tbot\n" << "REF2B\t0.00\t0.00\t0.00\t1mm\tCircle\tbot\n\n";
    }
    outputFile.close();

    QString st, st1, pos;
    QStringList listSort, listSort1;
    QFile inputFile(inputFileName);
    QFile outputFile("out.txt");
    QTextStream inputStream(&outputFile);
    qint32 count = 0, topCount = 0, botCount = 0;
    //=============== Open file for write =================

    if (inputFileName !="") //If input file name not null, then open input file
    {
        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text) && outputFile.open(QIODevice::Append))
        {
           //Read source file

            while (!inputFile.atEnd())
            {

                QString name = inputFile.readLine();

                listSort = name.split(QRegExp("\\s\\t"),QString::SkipEmptyParts);

                pos = listSort.at(8);
                if (pos == "YES\n")
                {
                    pos = "top";
                    topCount++;
                }
                else
                {
                    pos = "bot";
                    botCount++;
                }

                st = (listSort.at(0) + "\t" + listSort.at(5) + "\t" + listSort.at(6) + "\t" + listSort.at(7)
                + "\t" + listSort.at(2) + "\t" + listSort.at(1) + "\t" + pos) + "\r\n";

                inputStream << st;
                count++;

            }
            ui->labelTopCount->setNum(topCount);
            ui->labelBotCount->setNum(botCount);
            ui->label_2->setNum(count);

        }
        else
        {
            QMessageBox::critical(this,tr("Error"),tr("Could not open file"));
            return;
        }
     }

//======== Files closed ========

    inputFile.close();
    outputFile.close();

//======== Write files in textEdit =========
    QFile outputFile1("out.txt");
    if (outputFile1.open(QIODevice::ReadWrite | QIODevice::Text))
    {
    QTextStream outPut1(&outputFile1);
    ui->textEdit->setText(outPut1.readAll());
    }
    outputFile1.close();

    QFile outputFile2("out.txt");
    if (outputFile2.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {

    }
    outputFile2.close();
}

//=====================================================
//=============== Generator HTML ======================
//=====================================================

void MainWindow::on_buttonGenerateHtml_clicked()
{

    if (inputFileHtml.open(QIODevice::ReadOnly) | outputFileHtml.open(QIODevice::ReadWrite))
        {
        QMessageBox::warning(this,
                                     "Ошибка файла",
                                     "Не удалось открыть файл",
                                     QMessageBox::Ok);
        }
    else
        {
        QXmlStreamReader html(&inputFileHtml);

        while (!html.atEnd() && !html.hasError())
            {
                QXmlStreamReader::TokenType token = html.readNext();
                if (token == QXmlStreamReader::StartDocument)
                continue;
                if (token == QXmlStreamReader::StartElement)
                    {
                    if (html.name() == "tr")
                    continue;
                    if (html.name() == "td")
                    XMLConf.append(parseTd (html));
                    }
                //ui->textEdit->setText(html.readAll());
            }
        }
}

//=====================================================
//=============== Save file ===========================
//=====================================================

void MainWindow::on_buttonSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

        if (fileName != "")
        {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                // error message
            }
            else
            {
                file.write(ui->textEdit->toPlainText().toUtf8());
                file.close();
            }
        }
}

//=====================================================
//==================== Exit programm ==================
//=====================================================

void MainWindow::on_buttonExit_clicked()
{
    QApplication::quit();
}
