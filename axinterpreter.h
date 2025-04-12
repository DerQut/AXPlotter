#ifndef AXINTERPRETER_H
#define AXINTERPRETER_H

#include <QMessageBox>

class ContentView;


class AXInterpreter : public QMessageBox
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
