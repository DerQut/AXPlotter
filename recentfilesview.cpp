#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QScrollArea>

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
    background->setStyleSheet("background-color: white");

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
    recentFileButton1 = new QPushButton("a", this);
    recentFileButton2 = new QPushButton("b", this);
    recentFileButton3 = new QPushButton("c", this);
    recentFileButton4 = new QPushButton("d", this);
    recentFileButton5 = new QPushButton("e", this);

    recentFileButton1->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");
    recentFileButton2->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");
    recentFileButton3->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");
    recentFileButton4->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");
    recentFileButton5->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");

    scrollLayout->addWidget(recentFileButton1);
    scrollLayout->addWidget(recentFileButton2);
    scrollLayout->addWidget(recentFileButton3);
    scrollLayout->addWidget(recentFileButton4);
    scrollLayout->addWidget(recentFileButton5);

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

    connect(recentFileButton1, SIGNAL(clicked()), this, SLOT(forceReadRecentFile1()));
    connect(recentFileButton2, SIGNAL(clicked()), this, SLOT(forceReadRecentFile2()));
    connect(recentFileButton3, SIGNAL(clicked()), this, SLOT(forceReadRecentFile3()));
    connect(recentFileButton4, SIGNAL(clicked()), this, SLOT(forceReadRecentFile4()));
    connect(recentFileButton5, SIGNAL(clicked()), this, SLOT(forceReadRecentFile5()));
}


void RecentFilesView::updateButtons() {
    recentFileButton1->setText(contentView->recentFile1);
    recentFileButton2->setText(contentView->recentFile2);
    recentFileButton3->setText(contentView->recentFile3);
    recentFileButton4->setText(contentView->recentFile4);
    recentFileButton5->setText(contentView->recentFile5);
}


void RecentFilesView::forceReadRecentFile1() {
    this->contentView->readScriptFile(contentView->recentFile1);
}


void RecentFilesView::forceReadRecentFile2() {
    this->contentView->readScriptFile(contentView->recentFile2);
}


void RecentFilesView::forceReadRecentFile3() {
    this->contentView->readScriptFile(contentView->recentFile3);
}


void RecentFilesView::forceReadRecentFile4() {
    this->contentView->readScriptFile(contentView->recentFile4);
}


void RecentFilesView::forceReadRecentFile5() {
    this->contentView->readScriptFile(contentView->recentFile5);
}
