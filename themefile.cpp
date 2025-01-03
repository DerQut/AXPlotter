#include <string>
#include <fstream>

#include <QDebug>
#include <QString>

#include "themefile.h"

using namespace std;

Theme getTheme(const char* fileName) {

    string line;

    // Opening the given file
    ifstream file;
    file.open(fileName);

    if (!file.is_open()) {
        qDebug() << "Creating file";
        saveTheme(fileName, DEFAULT);
        return DEFAULT;
    }

    getline(file, line);
    file.close();
    qDebug() << "File read succesfully: " << QString::fromStdString(line);

    if (line == "DARK")
        return DARK;

    return DEFAULT;
}

void saveTheme(const char* fileName, Theme theme) {

    string line;

    switch (theme) {
    case DEFAULT:
        line = "DEFAULT";
        break;
    case DARK:
        line = "DARK";
        break;
    }

    // Opening the given file
    ofstream file;
    file.open(fileName);

    if (!file.is_open()) {
        qDebug() << "Error writing to file";
        return;
    }

    file << line;
    file.close();

    qDebug() << "Theme " << QString::fromStdString(line) << " saved.";
}

