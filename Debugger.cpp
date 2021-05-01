#include "Debugger.h"


void error(HRESULT hr) { error(hr, NULL); }
void error(HRESULT hr, const char* str) {
    if (SUCCEEDED(hr)) return;

    if (str == NULL) printf("\u001b[31mError\u001b[0m %x\n", hr);
    else printf("\u001b[31mError\u001b[0m [%s] %x\n", str, hr);

    exit(EXIT_FAILURE);
}

void log(const char* className, std::string str) { log(className, str.c_str()); }
void log(const char* className, const char* str) {
    std::string name(className);
    std::string text(str);

    name.insert(name.begin(), '[');
    name.insert(name.end(), ']');

    printf("%s %s\n", name.c_str(), text.c_str());
}