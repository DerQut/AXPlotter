#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QScrollArea>

#include "editorview.h"
#include "detailview.h"
#include "axsyntaxhighlighter.h"
#include "linenumbersview.h"

EditorView::EditorView(DetailView *parent) :
    QWidget(parent)
{
    detailView = parent;

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    QHBoxLayout* mainHStack = new QHBoxLayout();
    mainHStack->setMargin(0);
    mainHStack->setSpacing(0);

    lineNumbersView = new LineNumbersView(this);
    mainHStack->addWidget(lineNumbersView);

    textEdit = new QTextEdit(this);
    textEdit->setFont(font);
    textEdit->setLineWrapMode(QTextEdit::NoWrap);
    mainHStack->addWidget(textEdit);

    highlighter = new AXSyntaxHighlighter(textEdit->document());

    this->setLayout(mainHStack);
}


void EditorView::readRecipeFile(QString path) {

    QFile file(path);

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        textEdit->setPlainText(file.readAll());
        file.close();
    } else {
        qDebug() << "Reading of file \"" << path << "\" failed";
        return;
    }
}
