#ifndef SIDEBARVIEW_H
#define SIDEBARVIEW_H

#include <QWidget>

class SideBarView : public QWidget
{
    Q_OBJECT
public:
    explicit SideBarView(QWidget *parent = 0);

private:
    QWidget* background;

signals:

public slots:
    void setDefaultTheme(int shouldSet);
    void setDarkTheme(int shouldSet);

};

#endif // SIDEBARVIEW_H
