#ifndef RECENTFILESVIEW_H
#define RECENTFILESVIEW_H

#include <QWidget>
#include <QList>
#include <QPushButton>

#include "toggleviewhost.h"

class ContentView;


class RecentFilesView : public ToggleViewHost
{
    Q_OBJECT
public:
    explicit RecentFilesView(ContentView* parent);

    QList<QPushButton*> recentFileButtons;
    QWidget* scrollWidget;

signals:

public slots:
    void updateButtons();
    void forceReadRecentFile(int fileID);

};

#endif // RECENTFILESVIEW_H
