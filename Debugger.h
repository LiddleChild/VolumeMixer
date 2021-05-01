#pragma once

#include <winerror.h>
#include <iostream>
#include <string>

void error(HRESULT hr);
void error(HRESULT hr, const char*);

void log(const char*, const char*);
void log(const char*, std::string);