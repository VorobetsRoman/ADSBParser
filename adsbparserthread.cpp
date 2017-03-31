#include "adsbparserthread.h"
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

struct Record {qint32 icao; double lat; double lon;};

AdsbParserThread::AdsbParserThread(QObject *parent) : QObject(parent)
{}



void AdsbParserThread::parsing(QStringList *fileList, QFile *outFile)
{
    outFile->open(QIODevice::WriteOnly);
//    QTextStream out(outFile);

    float fileListSize = fileList->size();
    int fileCounter {0};

    for(QString fileName : *fileList)
    {
        fileCounter++;
        emit progress(int (100 * fileCounter / fileListSize));

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
        outFile->write((char*)&time, sizeof(time));

        // Закинули нужные данные из файла в память
        QList <Record*> records;
        foreach (QJsonValue jArrValue, jArr)
        {
            jObj = jArrValue.toObject();
            if (jObj.contains("Lat") && jObj.contains("Long"))
            {
                Record *record = new Record;
                record->icao    = jObj.value("Icao").toString().toInt(0, 16);
                record->lat     = jObj.value("Lat").toDouble();
                record->lon     = jObj.value("Long").toDouble();
                records.append(record);
            }
        }

        // Записали данные из памяти в файл
        qint32 recordsCount = records.count();
        outFile->write((char*)&recordsCount, sizeof(recordsCount));
        for (Record *record : records){
            outFile->write((char*)&(record->icao), sizeof(record->icao));
            outFile->write((char*)&(record->lat), sizeof(record->lat));
            outFile->write((char*)&(record->lon), sizeof(record->lon));
            delete record;
        }
        records.clear();
        qDebug() << "loaded" << fileName;
    }

    outFile->close();

    emit parsingDone();
}
