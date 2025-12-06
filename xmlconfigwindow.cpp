#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QStyle>
#include <QLineEdit>
#include <QDebug>

#include "xmlconfigwindow.h"

#include "contentview.h"

XMLConfigWindow::XMLConfigWindow(ContentView* parent) :
    QMainWindow(parent)
{
    this->contentView = parent;
    this->setWindowTitle("Devices XML config");

    QWidget* dummyWidget = new QWidget(this);

    QVBoxLayout* mainVStack = new QVBoxLayout(dummyWidget);
    mainVStack->setSpacing(0);

    QLabel* fileLabel = new QLabel("Devices XML file path:", this);
    fileLabel->setContentsMargins(0,0,0,0);
    mainVStack->addWidget(fileLabel);

    QHBoxLayout* fileHStack = new QHBoxLayout();
    fileHStack->setContentsMargins(0,0,0,0);

    fileDisplay = new QLineEdit(this->contentView->xmlFile, this);
    fileDisplay->setDisabled(true);
    fileHStack->addWidget(fileDisplay);

    QPushButton* fileButton = new QPushButton(this->style()->standardIcon(QStyle::SP_DirOpenIcon), "", this);
    fileButton->setToolTip("Select an .XML file");
    fileHStack->addWidget(fileButton);

    mainVStack->addLayout(fileHStack);
    mainVStack->addStretch();

    QCheckBox* useXMLCheckBox = new QCheckBox("Use an AIXACT Software .XML file for inferring device names", this);
    mainVStack->addWidget(useXMLCheckBox);

    this->setCentralWidget(dummyWidget);

    this->show();

    useXMLCheckBox->setChecked(contentView->doesUseXMLFile);

    connect(useXMLCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkboxAction(bool)));

    connect(fileButton, SIGNAL(released()), this->contentView, SLOT(obtainXMLFile()));

    connect(this->contentView, SIGNAL(xmlFileObtained(QString)), this->fileDisplay, SLOT(setText(QString)));
}


void XMLConfigWindow::checkboxAction(bool isChecked) {
    this->contentView->doesUseXMLFile = isChecked;
    this->contentView->saveXMLConfig();
}
