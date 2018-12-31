#include "pch.h"

#include "file_utils.h"

#include <stdio.h>
#include <stdlib.h>

namespace core {

	char* ReadFullFile(const char* filePath)
	{
		FILE* file = fopen(filePath, "rb");
		if (!file)
		{
			CORE_ERROR("ReadFullFile error: could not open file.");
			return nullptr;
		}
		fseek(file, 0, SEEK_END);
		long size = ftell(file);
		fseek(file, 0, SEEK_SET);

		char* fileString = new char[size + 1];
		fread(fileString, size, 1, file);
		fclose(file);
		fileString[size] = 0;

		return fileString;
	}

}