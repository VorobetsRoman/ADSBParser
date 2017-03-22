#include "adsbparser.h"
#include "ui_adsbparser.h"
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
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
    if (fileNames.isEmpty()) {
        return;
    }

    ui->pbFileSelect->setDisabled(true);
    ui->pbFileSelect->setText("working");

    QFile outFile("adsb.txt");
    outFile.open(QIODevice::WriteOnly);

    QTextStream out(&outFile);

    for(QString fileName : fileNames)
    {
        QFile adsbFile(fileName);
        if (!adsbFile.open(QIODevice::ReadOnly)) {
            qDebug() << "error while reading" << fileName;
            continue;
        }

        QByteArray ba = adsbFile.readAll();
        adsbFile.close();

        if (ba.isEmpty()) {
            qDebug() << "error reading file" << fileName;
            continue;
        }

        QJsonParseError *parserError {NULL};
        QJsonDocument jDoc = QJsonDocument::fromJson(ba, parserError);
        if (parserError) {
            qDebug() << parserError << fileName;
            continue;
        }

        QJsonObject jObj = jDoc.object();
        QJsonValue  jVal = jObj.value("acList");
        QJsonArray  jArr = jVal.toArray();


        quint32 time = jObj.value("stm").toVariant().toLongLong() / 1000;
        out << time;

        double lat {0}, lon {0};
        int icao {0};

        foreach (QJsonValue jArrValue, jArr) {
            jObj = jArrValue.toObject();
            if (jObj.contains("Lat") && jObj.contains("Long"))
            {
                icao    = jObj.value("Icao").toString().toInt(0, 16);
                lat     = jObj.value("Lat").toDouble();
                lon     = jObj.value("Long").toDouble();

                out << char(9) << icao << char(9) << lat << char(9) << lon;
            }
        }
        out << char(10);
        qDebug() << "loaded" << fileName;
    }

    out.flush();
    outFile.close();

    ui->pbFileSelect->setDisabled(false);
    ui->pbFileSelect->setText("Select files");
}




void AdsbParser::on_pbStart_released()
{

}
