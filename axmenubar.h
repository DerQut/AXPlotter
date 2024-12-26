#ifndef AXMENUBAR_H
#define AXMENUBAR_H

#include <QWidget>
#include <QMenuBar>

class AXMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit AXMenuBar(QWidget *parent = 0);

signals:

public slots:
    void showFileMenu();

};

#endif // AXMENUBAR_H
