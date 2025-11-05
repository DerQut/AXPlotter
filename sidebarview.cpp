#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedLayout>
#include <QGradient>
#include <QLineEdit>
#include <QPushButton>
#include <QStyle>

#include "sidebarview.h"
#include "contentview.h"
#include "recentfilesview.h"
#include "themefile.h"
#include "inferredvariablesview.h"

SideBarView::SideBarView(ContentView *parent) :
    QWidget(parent)
{
    this->contentView = parent;
    this->setContentsMargins(0, 0, 0, 0);

    QStackedLayout* mainZStack = new QStackedLayout();
    mainZStack->setStackingMode(QStackedLayout::StackAll);

    // Creating the widget to host the background gradient
    background = new QWidget(this);

    // Creating the main VStack to host widgets in front of the background
    QVBoxLayout* mainVStack = new QVBoxLayout();

    // Creating the label to show the current file directory
    QHBoxLayout* fileHStack = new QHBoxLayout();

    fileDisplay = new QLineEdit("Please select a file", this);
    fileDisplay->setDisabled(true);
    fileHStack->addWidget(fileDisplay);

    QPushButton* fileButton = new QPushButton(this->style()->standardIcon(QStyle::SP_DirOpenIcon), "", this);
    fileButton->setToolTip("Open file");
    fileHStack->addWidget(fileButton);

    mainVStack->addLayout(fileHStack);

    // Creating a recent files view
    recentFilesView = new RecentFilesView(this->contentView, this);
    mainVStack->addWidget(recentFilesView);

    inferredVariablesView = new InferredVariablesView(this->contentView);
    mainVStack->addWidget(inferredVariablesView);

    mainVStack->addStretch();

    QWidget* dummyZStackWidget = new QWidget();
    dummyZStackWidget->setLayout(mainVStack);

    mainZStack->addWidget(background);
    mainZStack->addWidget(dummyZStackWidget);

    this->setLayout(mainZStack);

    connect(fileButton, SIGNAL(released()), this->contentView, SLOT(obtainScriptFile()));
}


void SideBarView::setDarkTheme()
{
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:1, x2:0 y2:0, stop:0 #454545, stop:1 #808080);");
    saveTheme("theme.cfg", DARK);
}


void SideBarView::setDefaultTheme()
{
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:1, x2:0 y2:0, stop:0 #6375d7, stop:1 #7ba2e6);");
    saveTheme("theme.cfg", DEFAULT);
}


void SideBarView::setClassicTheme()
{
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:1, x2:0 y2:0, stop:0 #f5f5f5, stop:1 #f5f5f5);");
    saveTheme("theme.cfg", CLASSIC);
}


void SideBarView::setTheme(Theme theme) {
    switch (theme) {
    case CLASSIC:
        this->setClassicTheme();
        break;
    case DARK:
        this->setDarkTheme();
        break;
    default:
        this->setDefaultTheme();
    }
}


void SideBarView::setFileLabel(QString text) {
    this->fileDisplay->setText(text);
    this->fileDisplay->setToolTip(text);
}
