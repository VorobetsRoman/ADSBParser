#include "adsbparser.h"
#include "ui_adsbparser.h"
#include <QFile>
#include <QFileDialog>
#include <QDebug>




AdsbParser::AdsbParser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdsbParser)
{
    ui->setupUi(this);
}




AdsbParser::~AdsbParser()
{
    if (adsbParserThread) adsbParserThread->deleteLater();
    adsbParserThread = NULL;

    if (parserThread) {
        parserThread->quit();
        parserThread = NULL;
    }

    delete ui;
}




void AdsbParser::on_pbFileSelect_released()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(0, "input files", qApp->applicationDirPath(), "*.json", 0);
    if (fileNames.isEmpty()) {
        return;
    }

    ui->pbFileSelect->setDisabled(true);
    ui->pbFileSelect->setText("working");

    QFile outFile(fileNames.at(0).section("/", -1, -1).left(10));

    parserThread = new QThread();
    parserThread->start();

    adsbParserThread = new AdsbParserThread();
    connect(adsbParserThread, &AdsbParserThread ::parsingDone,
            this,   &AdsbParser ::parsingDone);

    adsbParserThread->moveToThread(parserThread);
    adsbParserThread->parsing(&fileNames, &outFile);
}



void AdsbParser::on_pbStart_released()
{

}




void AdsbParser::parsingDone()
{
    qDebug() << "parsing done recived";
    ui->pbFileSelect->setDisabled(false);
    ui->pbFileSelect->setText("Select files");

    if (adsbParserThread) adsbParserThread->deleteLater();
    adsbParserThread = NULL;

    if (parserThread) {
        parserThread->quit();
        parserThread = NULL;
    }
}
