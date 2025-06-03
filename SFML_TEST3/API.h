#pragma once
#ifdef _BUILD
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif // _BUILD


class EXPORT_API klasa
{
public:
	void test();
};