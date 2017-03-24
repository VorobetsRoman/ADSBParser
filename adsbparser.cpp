#include "adsbparser.h"
#include "ui_adsbparser.h"
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QProgressBar>
#include <QDir>
#include <QMessageBox>

#include "trajectory.h"




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
    fileNames = QFileDialog::getOpenFileNames(0, "input files", QDir::current().absolutePath(), "*.json", 0);
    if (fileNames.isEmpty()) {
        return;
    }

    ui->pbFileSelect->setDisabled(true);
    ui->pbFileSelect->setText("working");

    outFile = new QFile(fileNames.at(0).section("/", -1, -1).left(10) + ".adsb");

    parserThread = new QThread();

    adsbParserThread = new AdsbParserThread();
    connect(adsbParserThread,   &AdsbParserThread   ::parsingDone,
            this,               &AdsbParser         ::parsingDone);
    connect(parserThread,       &QThread            ::started,
            this,               &AdsbParser         ::startParsing);
    connect(adsbParserThread,   &AdsbParserThread   ::progress,
            ui->progressBar,    &QProgressBar       ::setValue);

    adsbParserThread->moveToThread(parserThread);
    parserThread->start();
}




void AdsbParser::startParsing()
{
    adsbParserThread->parsing(&fileNames, outFile);
}




void AdsbParser::parsingDone()
{
    ui->pbFileSelect->setDisabled(false);
    ui->pbFileSelect->setText("Select files");

    if (adsbParserThread) adsbParserThread->deleteLater();
    adsbParserThread = NULL;

    if (parserThread) {
        parserThread->quit();
        parserThread = NULL;
    }
}




void AdsbParser::on_pushButton_released()
{
    QString fileName = QFileDialog::getOpenFileName(0, "*adsb file", QDir::current().absolutePath(), "*.adsb", 0);
    if (fileName == "") return;

    QFile inFile(fileName);
    if (inFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0, "eror opening", "error opening your fuck'n file");
        return;
    }



    inFile.close();
}
