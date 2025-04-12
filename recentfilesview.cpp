#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QScrollArea>
#include <QButtonGroup>

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

    QPushButton* recentFileButton0 = new QPushButton("a", this);
    QPushButton* recentFileButton1 = new QPushButton("b", this);
    QPushButton* recentFileButton2 = new QPushButton("c", this);
    QPushButton* recentFileButton3 = new QPushButton("d", this);
    QPushButton* recentFileButton4 = new QPushButton("e", this);

    recentGroup->addButton(recentFileButton0, 0);
    recentGroup->addButton(recentFileButton1, 1);
    recentGroup->addButton(recentFileButton2, 2);
    recentGroup->addButton(recentFileButton3, 3);
    recentGroup->addButton(recentFileButton4, 4);

    recentFileButtons << recentFileButton0
                      << recentFileButton1
                      << recentFileButton2
                      << recentFileButton3
                      << recentFileButton4;

    recentFileButton0->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");
    recentFileButton1->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");
    recentFileButton2->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");
    recentFileButton3->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");
    recentFileButton4->setStyleSheet("border: none; color: palette(window-text); background: transparent; text-align:left;");

    scrollLayout->addWidget(recentFileButton0);
    scrollLayout->addWidget(recentFileButton1);
    scrollLayout->addWidget(recentFileButton2);
    scrollLayout->addWidget(recentFileButton3);
    scrollLayout->addWidget(recentFileButton4);

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

    connect(recentGroup, SIGNAL( buttonClicked(int) ), this, SLOT( forceReadRecentFile(int) ));
}


void RecentFilesView::updateButtons() {
    for (int i=0; i < recentFileButtons.count(); i++) {
        recentFileButtons[i]->setText( contentView->recentFiles[i] );
    }
}


void RecentFilesView::forceReadRecentFile(int fileID) {
    contentView->readScriptFile( contentView->recentFiles[fileID] );
}
