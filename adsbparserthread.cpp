#include "adsbparserthread.h"
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>



AdsbParserThread::AdsbParserThread(QObject *parent) : QObject(parent)
{

}



void AdsbParserThread::parsing(QStringList *fileList, QFile *outFile)
{
    outFile->open(QIODevice::WriteOnly);
    QTextStream out(outFile);

    for(QString fileName : *fileList)
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

        foreach (QJsonValue jArrValue, jArr)
        {
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
    outFile->close();

    emit parsingDone();
}
