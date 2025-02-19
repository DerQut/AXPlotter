#include <QWidget>
#include <QFont>
#include <QDebug>
#include <QVBoxLayout>
#include <QStackedLayout>

#include "linenumbersview.h"
#include "editorview.h"

LineNumbersView::LineNumbersView(EditorView *parent) :
    QWidget(parent)
{

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    QStackedLayout* mainZStack = new QStackedLayout();
    mainZStack->setStackingMode(QStackedLayout::StackAll);

    QWidget* background = new QWidget(this);
    background->setStyleSheet("background-color:lightGray;");
    mainZStack->addWidget(background);

    lineDisplay = new QTextEdit(this);
    lineDisplay->setFont(font);
    lineDisplay->setContentsMargins(0, 0, 0, 0);
    lineDisplay->setText("1");
    mainZStack->addWidget(lineDisplay);

    this->setLayout(mainZStack);

    this->setFixedWidth(20);
    this->setContentsMargins(0, 0, 0, 0);
}
