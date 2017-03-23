#ifndef ADSBPARSER_H
#define ADSBPARSER_H

#include <QWidget>
#include <QThread>
#include <QFile>
#include "adsbparserthread.h"

namespace Ui {
class AdsbParser;
}

class AdsbParser : public QWidget
{
    Q_OBJECT

public:
    explicit AdsbParser(QWidget *parent = 0);
    ~AdsbParser();

private slots:
    void on_pbFileSelect_released();

    void on_pbStart_released();

    void startParsing();
    void parsingDone();

private:
    Ui::AdsbParser      *ui;
    QThread             *parserThread {NULL};
    AdsbParserThread    *adsbParserThread {NULL};
    QStringList fileNames;
    QFile *outFile;
};

#endif // ADSBPARSER_H
