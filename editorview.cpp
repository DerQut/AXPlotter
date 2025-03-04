#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QScrollArea>

#include "editorview.h"
#include "detailview.h"
#include "axsyntaxhighlighter.h"

EditorView::EditorView(DetailView *parent) :
    QWidget(parent)
{
    detailView = parent;

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    QVBoxLayout* mainVStack = new QVBoxLayout();
    mainVStack->setMargin(0);
    mainVStack->setSpacing(0);

    textEdit = new QTextEdit(this);
    textEdit->setFont(font);
    textEdit->setLineWrapMode(QTextEdit::NoWrap);
    mainVStack->addWidget(textEdit);

    highlighter = new AXSyntaxHighlighter(textEdit->document());

    this->setLayout(mainVStack);
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
