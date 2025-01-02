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
        ofstream fallbackFile;
        fallbackFile.open(fileName);
        fallbackFile << "DEFAULT";
        fallbackFile.close();
        return DEFAULT;
    }

    getline(file, line);
    file.close();
    qDebug() << "File read succesfully: " << QString::fromStdString(line);

    if (line == "DARK")
        return DARK;

    return DEFAULT;
}
