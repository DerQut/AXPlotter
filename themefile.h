#ifndef THEMEFILE_H
#define THEMEFILE_H

#pragma once

#include <string>

enum Theme {
    DEFAULT,
    DARK
};

Theme getTheme(const char* fileName);

Theme themeFromString(std::string themeName);
std::string charFromTheme(Theme theme);

void saveTheme(const char* fileName, Theme theme);

#endif // THEMEFILE_H
