#ifndef AXINTERPRETER_H
#define AXINTERPRETER_H

#include <QMainWindow>

class ContentView;


class AXInterpreter : public QMainWindow
{
    Q_OBJECT
public:
    explicit AXInterpreter(ContentView* parent = 0);
    ContentView* contentView;

signals:
    void failure();
    void success();

public slots:
    void startCompilation();

};

#endif // AXINTERPRETER_H
