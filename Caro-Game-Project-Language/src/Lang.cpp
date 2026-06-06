#include "Lang.h"
#include <sstream>
#include <iostream>
#include <fstream>

static string trim(const string& text)
{
    const char* whitespace = " \t\r\n";
    size_t start = text.find_first_not_of(whitespace);
    if (start == string::npos)
        return "";
    size_t end = text.find_last_not_of(whitespace);
    return text.substr(start, end - start + 1);
}

void loadAllLanguage(LanguageBank &language) {
    loadLanguageFromFile(langFile, language.langEn, ENGLISH);
    loadLanguageFromFile(langFile, language.langVi, VIETNAMESE);
}

bool loadLanguageFromFile(const char* filename, map<string,string>& langMap, LanguageType languageIndex) {
    ifstream file;
    file.open(filename);
    if(!file.is_open()) {
        cout << " khong the doc duoc file " << filename << "\n";
        return false;
    }
    string line;
    while (getline(file,line)) {
        if (line.empty() || line.find("ITEM") == 0) {
            continue;
        }
        string item, translatedText;
        if (!parseRowData(line, languageIndex, item, translatedText)) {
            continue;
        }
        langMap[item] = translatedText;
    }
    return true;
}

bool parseRowData(string line, LanguageType languageIndex,string &item, string &translatedText ) {
    stringstream read(line);
    if (!getline(read, item, ',')) {
        return false;
    }
    item = trim(item);

    for (int i = 1; i <= languageIndex; i++) {
        if (!getline(read, translatedText, ',')) {
            return false;
        }
    }
    translatedText = trim(translatedText);
    return true;
}

const char* getText(const char *key, const map<string,string> &langMap) {
    if (key == nullptr) return "";
    string k(key);
    auto it = langMap.find(k);
    if (it != langMap.end()) {
        return it->second.c_str();
    }
    return key;
}
