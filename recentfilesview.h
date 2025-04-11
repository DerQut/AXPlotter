#ifndef RECENTFILESVIEW_H
#define RECENTFILESVIEW_H

#include <QWidget>
#include <QList>
#include <QPushButton>

class ContentView;


class RecentFilesView : public QWidget
{
    Q_OBJECT
public:
    explicit RecentFilesView(ContentView* contentView, QWidget *parent = 0);
    ContentView* contentView;

    QList<QPushButton*> recentFileButtons;

signals:

public slots:
    void updateButtons();
    void forceReadRecentFile(int fileID);

};

#endif // RECENTFILESVIEW_H
