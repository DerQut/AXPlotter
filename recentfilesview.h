#ifndef RECENTFILESVIEW_H
#define RECENTFILESVIEW_H

#include <QWidget>
#include <QPushButton>

class ContentView;


class RecentFilesView : public QWidget
{
    Q_OBJECT
public:
    explicit RecentFilesView(ContentView* contentView, QWidget *parent = 0);
    ContentView* contentView;

    // Temporary method, will probably be replaced with an array
    QPushButton* recentFileButton1;
    QPushButton* recentFileButton2;
    QPushButton* recentFileButton3;
    QPushButton* recentFileButton4;
    QPushButton* recentFileButton5;

signals:

public slots:
    void updateButtons();
    void forceReadRecentFile1();
    void forceReadRecentFile2();
    void forceReadRecentFile3();
    void forceReadRecentFile4();
    void forceReadRecentFile5();

};

#endif // RECENTFILESVIEW_H
