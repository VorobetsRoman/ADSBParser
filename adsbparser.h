#ifndef ADSBPARSER_H
#define ADSBPARSER_H

#include <QWidget>

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

private:
    Ui::AdsbParser *ui;
};

#endif // ADSBPARSER_H
