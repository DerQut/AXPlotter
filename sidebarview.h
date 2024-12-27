#ifndef SIDEBARVIEW_H
#define SIDEBARVIEW_H

#include <QWidget>

#include "contentview.h"

class SideBarView : public QWidget
{
    Q_OBJECT
public:
    explicit SideBarView(ContentView *parent = 0);

signals:

public slots:

};

#endif // SIDEBARVIEW_H
