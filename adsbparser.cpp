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
    delete ui;
}




void AdsbParser::on_pbFileSelect_released()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(0, "input files", qApp->applicationDirPath(), "*.json", 0);

    for(QString fileName : fileNames)
    {
        QFile adsbFile(fileName);
        if (!adsbFile.open(QIODevice::ReadOnly)) {
            qDebug() << "error while reading" << fileName;
            continue;
        }



        adsbFile.close();
    }
}




void AdsbParser::on_pbStart_released()
{

}
