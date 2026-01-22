#include "helpwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

#include "contentview.h"

HelpWindow::HelpWindow(ContentView* parent) :
    QMainWindow(parent)
{
    this->hide();

    QWidget* mainWidget = new QWidget(parent);
    QVBoxLayout* mainVStack = new QVBoxLayout();

    QScrollArea* scrollArea = new QScrollArea(parent);
    scrollArea->setWidgetResizable(true);

    QLabel* label = new QLabel(
        "<h1>AXPlotter v1.0.0</h1>"
        "<hr>"
        "   <p>AXPlotter is a Windows XP program designed to simulate time series of variable and device values based on AIXACT Software recipe scripts.</p>"
    );
    label->setWordWrap(true);

    mainVStack->addWidget(label);
    mainVStack->addStretch();

    mainWidget->setLayout(mainVStack);

    scrollArea->setWidget(mainWidget);
    this->setCentralWidget(scrollArea);

}
