#ifndef AXINTERPRETER_H
#define AXINTERPRETER_H

#include <QMainWindow>
#include <QDir>

class ContentView;


class AXInterpreter : public QMainWindow
{
    Q_OBJECT
public:
    explicit AXInterpreter(ContentView* parent = 0);
    ContentView* contentView;
    QDir baseFolder;
    QString scriptFile;

signals:
    void failure();
    void success();

public slots:
    void startCompilation(QString scriptFile);
    int recreateFolder();

};

#endif // AXINTERPRETER_H
