#pragma once

#ifdef _WIN32
#define CM_API __declspec(dllexport)
#else
#define CM_API
#endif