#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>

#include "inferredvariablesview.h"
#include "contentview.h"
#include "detailview.h"
#include "stackedgraphsview.h"
#include "toggleviewhost.h"
#include "clearlayout.h"

InferredVariablesView::InferredVariablesView(ContentView* parent) :
    ToggleViewHost(parent)
{
    this->toggleButton->setText("Inferred variables");

    this->scrollArea = new QScrollArea();
    this->scrollArea->setWidgetResizable(true);

    this->scrollWidget = new QWidget(this);

    QVBoxLayout* mainVStack = new QVBoxLayout();
    mainVStack->setContentsMargins(0,0,0,0);

    scrollVStack = new QVBoxLayout();
    scrollVStack->setContentsMargins(0,0,0,0);
    scrollWidget->setLayout(scrollVStack);

    scrollArea->setWidget(scrollWidget);

    mainVStack->addWidget(scrollArea);

    this->toggleView->setLayout(mainVStack);

    this->refreshVariables();

}

void InferredVariablesView::refreshVariables() {

    clearLayout(this->scrollVStack);

    for (std::int_fast64_t i=0; i < this->contentView->inferredVariables.count(); i++) {
        QWidget* rowHost = new QWidget();
        rowHost->setContentsMargins(0,0,0,0);

        QHBoxLayout* rowHStack = new QHBoxLayout();
        rowHStack->setContentsMargins(0,0,0,0);

        if (i & 1) {
            rowHost->setStyleSheet("background-color: #ffffff;");
        } else {
            rowHost->setStyleSheet("background-color: #f8f8f8;");
        }

        QLabel* nameLabel = new QLabel(this->contentView->inferredVariables[i].variableName);
        nameLabel->setStyleSheet("color: black; padding: 5px;");
        qDebug() << this->contentView->inferredVariables[i].variableName;

        rowHStack->addWidget(nameLabel);
        rowHStack->addStretch();

        QLineEdit* multiplierEdit = new QLineEdit("1", this);
        multiplierEdit->setFixedWidth(50);
        multiplierEdit->setContentsMargins(0,0,5,0);
        rowHStack->addWidget(multiplierEdit);

        QObject::connect(multiplierEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
            contentView->inferredVariables[i].trySetMultiplier(text);
            contentView->detailView->stackedGraphsView->replot();
        });

        rowHost->setLayout(rowHStack);

        this->scrollVStack->addWidget(rowHost);
    }
    this->scrollVStack->addStretch();
}

void InferredVariablesView::setClassicTheme() {
    ToggleViewHost::setClassicTheme();
    this->scrollArea->setStyleSheet("QScrollArea{}");
}

void InferredVariablesView::setDarkTheme() {
    ToggleViewHost::setDarkTheme();
    this->scrollArea->setStyleSheet("QScrollArea{border: none;}");
}

void InferredVariablesView::setDefaultTheme() {
    ToggleViewHost::setDefaultTheme();
    this->scrollArea->setStyleSheet("QScrollArea{border: none};");
}
