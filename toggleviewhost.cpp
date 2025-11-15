#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "toggleviewhost.h"
#include "contentview.h"

ToggleViewHost::ToggleViewHost(ContentView* parent) :
    QWidget(parent)
{
    this->contentView = parent;
    this->setObjectName("toggleViewHost");

    QVBoxLayout* mainVStack = new QVBoxLayout(this);
    mainVStack->setContentsMargins(0, 0, 0, 0);
    mainVStack->setSpacing(0);
    this->setLayout(mainVStack);

    this->toggleButton = new QPushButton("Name", this);
    this->toggleButton->setObjectName("toggleButton");

    mainVStack->addWidget(toggleButton);

    connect(this->toggleButton, SIGNAL( clicked() ), this, SLOT( toggle() ));

    this->toggleView = new QWidget(this);

    toggleView->setContentsMargins(0, 0, 0, 0);
    mainVStack->addWidget(toggleView);

    this->setContentsMargins(0, 0, 0, 0);

}

void ToggleViewHost::toggle() {

    if (this->toggleView->isVisible()) {
        this->toggleView->hide();
        emit hidden();
    } else {
        this->toggleView->show();
        emit shown();
    }
}

void ToggleViewHost::setDefaultTheme() {
    const QString toggleButtonStylesheet = QString(
        "QPushButton#toggleButton {"
            "border: none;"
            "color: black;"
            "text-align:left;"
            "padding-left: 10px;"
            "padding-right: 10px;"
            "padding-top: 5px;"
            "padding-bottom: 5px;"
            "font: bold;"
            "background-color: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 #ffffff, stop:1 #c9d5fa);"
            "border-top-left-radius: 5px;"
            "border-top-right-radius: 5px;"
        "}"
        "QPushButton#toggleButton:focus {"
            "outline: none;"
        "}"
        "QPushButton#toggleButton:pressed {"
            "background-color: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 #dedede, stop:1 #afb8d9);"
        "}"
    );

    this->toggleButton->setStyleSheet(toggleButtonStylesheet);
}


void ToggleViewHost::setClassicTheme() {
    const QString toggleButtonStylesheet = QString(
        "QPushButton#toggleButton {"
            "border:none;"
            "color: white;"
            "text-align:left;"
            "padding-left: 10px;"
            "padding-right: 10px;"
            "padding-top: 5px;"
            "padding-bottom: 5px;"
            "font: bold;"
            "background-color: #0a2364;"
        "}"
        "QPushButton#toggleButton:focus {"
            "outline: none;"
        "}"
        "QPushButton#toggleButton:pressed {"
            "background-color: #001049;"
        "}"
    );

    this->toggleButton->setStyleSheet(toggleButtonStylesheet);

}


void ToggleViewHost::setDarkTheme() {
    const QString toggleButtonStylesheet = QString(
        "QPushButton#toggleButton {"
            "border: none;"
            "color: white;"
            "text-align:left;"
            "padding-left: 10px;"
            "padding-right: 10px;"
            "padding-top: 5px;"
            "padding-bottom: 5px;"
            "font: bold;"
            "background-color: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 #4b4b4b, stop:1 #141414);"
            "border-top-left-radius: 5px;"
            "border-top-right-radius: 5px;"
        "}"
        "QPushButton#toggleButton:focus {"
            "outline: none;"
        "}"
        "QPushButton#toggleButton:pressed {"
            "background-color: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 #323232, stop:1 #020202);"
        "}"
    );

    this->toggleButton->setStyleSheet(toggleButtonStylesheet);

}
