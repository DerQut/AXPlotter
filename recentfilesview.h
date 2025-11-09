#ifndef RECENTFILESVIEW_H
#define RECENTFILESVIEW_H

#include <QWidget>
#include <QList>
#include <QPushButton>
#include <QScrollArea>
#include <QResizeEvent>

#include "toggleviewhost.h"

class ContentView;


class RecentFilesView : public ToggleViewHost
{
    Q_OBJECT
public:
    explicit RecentFilesView(ContentView* parent);

    QList<QPushButton*> recentFileButtons;
    QWidget* scrollWidget;
    QScrollArea* scrollArea;

signals:

public slots:
    void updateButtons();
    void forceReadRecentFile(int fileID);
    void updateButtonSize();

protected:
    void resizeEvent(QResizeEvent* event) override;

};

#endif // RECENTFILESVIEW_H
