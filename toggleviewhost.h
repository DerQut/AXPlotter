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

public slots:
    void toggle();

};

#endif // TOGGLEVIEWHOST_H
