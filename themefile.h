#include <string>

#ifndef THEMEFILE_H
#define THEMEFILE_H

enum Theme {
    DEFAULT,
    DARK
};

Theme getTheme(const char *fileName);

#endif // THEMEFILE_H
