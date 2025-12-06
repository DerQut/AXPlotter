#ifndef XMLCONFIGWINDOW_H
#define XMLCONFIGWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

class ContentView;

class XMLConfigWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit XMLConfigWindow(ContentView *parent = 0);

    ContentView* contentView;
    QLineEdit* fileDisplay;

signals:

public slots:
    void checkboxAction(bool isChecked);

};

#endif // XMLCONFIGWINDOW_H
