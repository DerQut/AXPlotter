#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QScrollArea>
#include <QButtonGroup>
#include <QDebug>

#include "recentfilesview.h"
#include "contentview.h"

RecentFilesView::RecentFilesView(ContentView* contentView, QWidget* parent) :
    QWidget(parent)
{
    this->contentView = contentView;

    QStackedLayout* mainZStack = new QStackedLayout();
    mainZStack->setStackingMode(QStackedLayout::StackAll);

    // Creating the widget to host the background gradient
    QWidget* background = new QWidget(this);
    background->setStyleSheet("background-color: #f5f5f5");

    // Creating the main VStack to host widgets in front of the background
    QVBoxLayout* mainVStack = new QVBoxLayout();

    // Creating a main label
    QLabel* label = new QLabel("Recent files:", this);
    label->setStyleSheet("font-weight: bold;");
    mainVStack->addWidget(label);

    // Creating a QScrollArea instance
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* scrollWidget = new QWidget();
    QVBoxLayout* scrollLayout = new QVBoxLayout();

    // Creating button instances
    QButtonGroup* recentGroup = new QButtonGroup(this);
    recentGroup->setExclusive(0);

    const QString stylesheet = QString(
        "QPushButton {"
            "border: none;"
            "color: palette(window-text);"
            "background: transparent;"
            "text-align:left;"
        "}"
            "QTooltip {"
                "border: 1px;"
                "color: black;"
                "background: white;"
        "}"
    );

    for (int i=0; i<this->contentView->recentFiles.count(); i++) {

        QPushButton* button = new QPushButton("", this);
        button->setStyleSheet(stylesheet);

        recentFileButtons << button;

        recentGroup->addButton(button, i);
        scrollLayout->addWidget(button);
    }

    scrollWidget->setLayout(scrollLayout);
    scrollWidget->setFixedWidth(300);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(scrollWidget);

    mainVStack->addWidget(scrollArea);

    // Creating a dummy widget to host the main VStack
    QWidget* dummyWidget = new QWidget();
    dummyWidget->setLayout(mainVStack);

    mainZStack->addWidget(background);
    mainZStack->addWidget(dummyWidget);

    this->setLayout(mainZStack);

    this->setMaximumHeight(160);

    connect(this->contentView, SIGNAL(recentFilesChanged()), this, SLOT(updateButtons()));
    this->updateButtons();

    connect(recentGroup, SIGNAL( buttonClicked(int) ), this, SLOT( forceReadRecentFile(int) ));
}


void RecentFilesView::updateButtons() {
    for (int i=0; i < recentFileButtons.count(); i++) {
        recentFileButtons[i]->setText( contentView->recentFiles[i] );
        recentFileButtons[i]->setToolTip( contentView->recentFiles[i] );
        recentFileButtons[i]->setDisabled(false);
        if (contentView->recentFiles[i] == "") {
            recentFileButtons[i]->setDisabled(true);
        }
    }
}


void RecentFilesView::forceReadRecentFile(int fileID) {
    contentView->readScriptFile( contentView->recentFiles[fileID] );

    // Shift the clicked file to the first position of this->contentView->recentFiles
    contentView->recentFiles.move(fileID, 0);
    this->updateButtons();
}
