#ifndef AXTOOLBAR_H
#define AXTOOLBAR_H

#include <QToolBar>

#include "contentview.h"

class AXToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit AXToolBar(ContentView *parent = 0);
    ContentView* contentView;

signals:

public slots:

};

#endif // AXTOOLBAR_H
