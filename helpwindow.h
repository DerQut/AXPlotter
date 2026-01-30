#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QMainWindow>

class ContentView;

class HelpWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit HelpWindow(ContentView* parent = 0);

signals:

public slots:

};

#endif // HELPWINDOW_H
