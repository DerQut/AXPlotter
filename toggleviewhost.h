#ifndef TOGGLEVIEWHOST_H
#define TOGGLEVIEWHOST_H

#include <QWidget>
#include <QPushButton>

class ContentView;

class ToggleViewHost : public QWidget
{
    Q_OBJECT
public:
    explicit ToggleViewHost(ContentView* parent = 0);
    ContentView* contentView;

    QWidget* toggleView;
    QPushButton* toggleButton;

signals:
    void hidden();
    void shown();

public slots:
    void toggle();
    void setDarkTheme();
    void setDefaultTheme();
    void setClassicTheme();

};

#endif // TOGGLEVIEWHOST_H
