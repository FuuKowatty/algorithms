#ifndef C_STRING
#define C_STRING

#include <stdio.h>

typedef struct {
    char *str;
    size_t length;
} string;

string *newString(const char *staticString);

void freeString(string *str);

string *concat(string *str1, string *str2);

string *trim(string *str);

string *replace(string *str, const char oldChar,  const char newChar);

string *toUpper(string *str);

string *toLower(string *str);

string *reverse(string *str);

string *extract(string *str, size_t start, size_t end);

int compare(string *str1, string *str2);

string *copyString(string *str);

int isSubstring(string *str, string *sub);

string **split(string *str, const char delimeter, size_t *count);

int compareChar(string *str1, char *str2);

#endif