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

    // Error handling: file missing
    if (!file.is_open()) {
        qDebug() << "Creating file";
        saveTheme(fileName, DEFAULT);
        return DEFAULT;
    }

    // Reading data from file
    getline(file, line);
    file.close();
    qDebug() << "File read succesfully: " << QString::fromStdString(line);

    // Parsing std::string to Theme
    if (line == "DARK")
        return DARK;

    return DEFAULT;
}

void saveTheme(const char* fileName, Theme theme) {

    string line;

    // Parsing Theme to std::string
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

    // Error handling
    if (!file.is_open()) {
        qDebug() << "Error writing to file";
        return;
    }

    // Reading Theme to file
    file << line;
    file.close();

    qDebug() << "Theme " << QString::fromStdString(line) << " saved.";
}

