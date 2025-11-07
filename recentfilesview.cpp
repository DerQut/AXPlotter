#include <cstdint>

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QScrollArea>
#include <QButtonGroup>
#include <QDebug>

#include "recentfilesview.h"
#include "contentview.h"
#include "toggleviewhost.h"

RecentFilesView::RecentFilesView(ContentView* parent) :
    ToggleViewHost(parent)
{
    this->contentView = parent;

    this->toggleButton->setText("Recent files");

    // Creating the main VStack to host widgets in front of the background
    QVBoxLayout* mainVStack = new QVBoxLayout();
    mainVStack->setContentsMargins(0, 0, 0, 0);

    // Creating a QScrollArea instance
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setContentsMargins(0,0,0,0);

    scrollWidget = new QWidget();
    scrollWidget->setContentsMargins(0,0,0,0);

    QVBoxLayout* scrollLayout = new QVBoxLayout();
    scrollLayout->setContentsMargins(0,0,0,0);
    scrollLayout->setSpacing(0);

    // Creating button instances
    QButtonGroup* recentGroup = new QButtonGroup(this);
    recentGroup->setExclusive(0);

    const QString buttonStylesheet = QString(
        "QPushButton {"
            "border: none;"
            "color: palette(window-text);"
            "background: transparent;"
            "text-align:left;"
            "padding: 5px;"
        "}"
            "QTooltip {"
                "border: 1px;"
                "color: palette(window-text);"
                "background: white;"
        "}"
    );

    // Create the buttons
    for (std::uint_fast8_t i=0; i<this->contentView->recentFiles.count(); i++) {

        QHBoxLayout* buttonHStack = new QHBoxLayout();
        buttonHStack->setContentsMargins(0,0,0,0);

        QWidget* dummyWidget = new QWidget(this);

        if (i & 1) {
            dummyWidget->setStyleSheet("background-color: #ffffff");
        } else {
            dummyWidget->setStyleSheet("background-color: #f8f8f8");
        }

        QPushButton* button = new QPushButton("", this);
        button->setStyleSheet(buttonStylesheet);

        button->setContentsMargins(0,0,0,0);

        recentFileButtons << button;
        recentGroup->addButton(button, i);

        buttonHStack->addWidget(button);
        buttonHStack->addStretch();

        dummyWidget->setLayout(buttonHStack);

        scrollLayout->addWidget(dummyWidget);
    }

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(scrollWidget);

    qDebug() << scrollArea->styleSheet();

    const QString scrollAreaStylesheet = QString(
        "QScrollArea{"
            "border: none;"
        "}"
    );

    scrollArea->setStyleSheet(scrollAreaStylesheet);

    mainVStack->addWidget(scrollArea);

    this->toggleView->setLayout(mainVStack);

    this->setMaximumHeight(160);

    connect(this->contentView, SIGNAL(recentFilesChanged()), this, SLOT(updateButtons()));
    this->updateButtons();

    connect(recentGroup, SIGNAL( buttonClicked(int) ), this, SLOT( forceReadRecentFile(int) ));
}


void RecentFilesView::updateButtons() {

    int longest = 0;

    for (std::uint_fast8_t i=0; i < recentFileButtons.count(); i++) {
        recentFileButtons[i]->setText( contentView->recentFiles[i] );
        recentFileButtons[i]->setToolTip( contentView->recentFiles[i] );
        recentFileButtons[i]->setDisabled(false);
        if (contentView->recentFiles[i] == "") {
            recentFileButtons[i]->setDisabled(true);
        }

        if (recentFileButtons[i]->text().count() > longest) {
            longest = recentFileButtons[i]->text().count();
        }
        if (longest*7 > 175) {
            scrollWidget->setFixedWidth(longest*7);
        } else {
            scrollWidget->setFixedWidth(175);
        }
    }
}


void RecentFilesView::forceReadRecentFile(int fileID) {
    contentView->readScriptFile( contentView->recentFiles[fileID] );

    // Shift the clicked file to the first position of this->contentView->recentFiles
    contentView->recentFiles.move(fileID, 0);
    this->updateButtons();
}
