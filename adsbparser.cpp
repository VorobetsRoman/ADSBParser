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
    fileNames = QFileDialog::getOpenFileNames(0, "input files", qApp->applicationDirPath(), "*.json", 0);
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

    adsbParserThread->moveToThread(parserThread);
    parserThread->start();
}




void AdsbParser::startParsing()
{
    adsbParserThread->parsing(&fileNames, outFile);
}




void AdsbParser::on_pbStart_released()
{

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




