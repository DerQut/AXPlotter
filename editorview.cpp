#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>

#include "editorview.h"
#include "detailview.h"

EditorView::EditorView(DetailView *parent) :
    QWidget(parent)
{
    detailView = parent;

    QVBoxLayout* mainVStack = new QVBoxLayout();
    mainVStack->setMargin(0);

    textEdit = new QTextEdit(this);
    mainVStack->addWidget(textEdit);

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
