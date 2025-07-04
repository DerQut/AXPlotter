#ifndef AXINTERPRETER_H
#define AXINTERPRETER_H

#include <QMainWindow>
#include <QDir>
#include <QLabel>

class ContentView;


class AXInterpreter : public QMainWindow
{
    Q_OBJECT
public:
    explicit AXInterpreter(ContentView* parent = 0);
    QLabel* mainText;
    ContentView* contentView;
    QDir baseFolder;
    QString scriptFile;
    QDir scriptFileFolder;

signals:
    void failure();
    void success();

public slots:
    void startCompilation(QString scriptFile);
    int recreateFolder();
    int generateAXRfile();
    //int generateAXCfile();
    //int generateAXSfile();

};

#endif // AXINTERPRETER_H
