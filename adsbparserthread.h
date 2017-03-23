#ifndef ADSBPARSERTHREAD_H
#define ADSBPARSERTHREAD_H

#include <QObject>
#include <QFile>

class AdsbParserThread : public QObject
{
    Q_OBJECT
public:
    explicit AdsbParserThread(QObject *parent = 0);

signals:
    void parsingDone ();

public slots:
    void parsing(QStringList *fileList, QFile* outFile);
};

#endif // ADSBPARSERTHREAD_H
