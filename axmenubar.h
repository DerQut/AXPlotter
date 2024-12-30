#ifndef AXMENUBAR_H
#define AXMENUBAR_H

#include <QWidget>
#include <QMenuBar>
#include <QComboBox>

#include "contentview.h"

class AXMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit AXMenuBar(ContentView* parent = 0);
    ContentView* contentView;

private:
    QComboBox* themePicker;

signals:

public slots:
    void showFileMenu();
    void showThemeMenu();

};

#endif // AXMENUBAR_H
