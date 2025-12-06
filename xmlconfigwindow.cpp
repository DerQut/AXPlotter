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

    QWidget* dummyWidget = new QWidget(this);

    QVBoxLayout* mainVStack = new QVBoxLayout(dummyWidget);

    QCheckBox* useXMLCheckBox = new QCheckBox("Use an AIXACT Software .XML file for inferring variables", this);
    mainVStack->addWidget(useXMLCheckBox);

    QHBoxLayout* fileHStack = new QHBoxLayout();

    fileDisplay = new QLineEdit(this->contentView->xmlFile, this);
    fileDisplay->setDisabled(true);
    fileHStack->addWidget(fileDisplay);

    QPushButton* fileButton = new QPushButton(this->style()->standardIcon(QStyle::SP_DirOpenIcon), "", this);
    fileButton->setToolTip("Select an .XML file");
    fileHStack->addWidget(fileButton);

    mainVStack->addLayout(fileHStack);

    this->setCentralWidget(dummyWidget);

    this->hide();

    useXMLCheckBox->setChecked(contentView->doesUseXMLFile);

    connect(useXMLCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkboxAction(bool)));

    connect(fileButton, SIGNAL(released()), this->contentView, SLOT(obtainXMLFile()));

    connect(this->contentView, SIGNAL(xmlFileObtained(QString)), this->fileDisplay, SLOT(setText(QString)));
}


void XMLConfigWindow::checkboxAction(bool isChecked) {
    this->contentView->doesUseXMLFile = isChecked;
    this->contentView->saveXMLConfig();
}
