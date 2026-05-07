#pragma once

#include <string>
#include <map>
using namespace std;

enum LanguageType {
    
    ENGLISH = 1,
    VIETNAMESE
};

struct  LanguageBank {
    map<string,string> langEn;
    map<string,string> langVi;
};

static const char* langFile = "language/lang.csv";

void loadAllLanguage(LanguageBank &language);
bool loadLanguageFromFile(const char* file, map<string,string>& langMap, LanguageType lang);
bool parseRowData(string line, LanguageType languageIndex,string &item, string &translatedText );
const char* getText(const char *text, const map<string,string> &langMap);
