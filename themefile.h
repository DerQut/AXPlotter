#ifndef THEMEFILE_H
#define THEMEFILE_H

enum Theme {
    DEFAULT,
    DARK
};

Theme getTheme(const char* fileName);

void saveTheme(const char* fileName, Theme theme);

#endif // THEMEFILE_H
