#include <string>
#include <fstream>

#include <QDebug>
#include <QString>

#include "themefile.h"

using namespace std;

Theme themeFromString(string themeName) {
    if (themeName == "CLASSIC") return CLASSIC;
    if (themeName == "DARK") return DARK;
    if (themeName == "GREEN") return GREEN;

    return DEFAULT;
}

string stringFromTheme(Theme theme) {
    switch(theme) {
    case(CLASSIC):
        return "CLASSIC";
        break;
    case(GREEN):
        return "GREEN";
        break;
    case(DARK):
        return "DARK";
        break;
    default:
        return "DEFAULT";
    }
}

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
    return themeFromString(line);
}

void saveTheme(const char* fileName, Theme theme) {

    string line;

    // Parsing Theme to std::string
    line = stringFromTheme(theme);

    // Opening the given file
    ofstream file;
    file.open(fileName);

    // Error handling
    if (!file.is_open()) {
        qDebug() << "Error writing to file";
        return;
    }

    // Saving Theme to file
    file << line;
    file.close();

    qDebug() << "Theme " << QString::fromStdString(line) << " saved.";
}

