#pragma once
#include "..\resource.h"
#include <Windows.h>

#include <iostream>
#include <string>
#include <string_view>
#include <vector>


class Resource {
public:
	struct Parameters {
		std::size_t size_bytes = 0;
		void* ptr = nullptr;
	};
private:
	HRSRC hResource = nullptr;
	HGLOBAL hMemory = nullptr;

	Parameters p;

public:
	Resource() { ; }

	Resource(int resource_id, const std::string& resource_class) {
		SetAll(resource_id, resource_class);
	}

	void SetAll(int resource_id, const std::string& resource_class) {
		hResource = FindResourceA(nullptr, MAKEINTRESOURCEA(resource_id), resource_class.c_str());
		hMemory = LoadResource(nullptr, hResource);

		p.size_bytes = SizeofResource(nullptr, hResource);
		p.ptr = LockResource(hMemory);
	}

	auto& GetResource() const {
		return p;
	}

	void GetInfo(int count = 0) const
	{
		std::cout << "\n" << count << ".\n" << p.ptr << ", " << p.size_bytes;
	}

	void /*auto*/ GetResourceString(int count = 0) const {
		std::string_view dst;
		if (p.ptr != nullptr)
			dst = std::string_view(reinterpret_cast<char*>(p.ptr), p.size_bytes);

		std::cout << "\n" << count << ".\n" << dst;
		//return dst;
	}

	void GetResourceInteger(int count = 0) const {
		std::cout << "\n" << count << ".\n";

		for (size_t i = 0; i < 10; i++)
			std::cout << p.ptr << " ";
	}
};
