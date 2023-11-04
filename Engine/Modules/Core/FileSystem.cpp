#include "FileSystem.h"
#include <ios>
#include <fstream>
#include <Log.h>

using namespace ECSEngine;

bool FileSystem::tryReadFile(const std::string& filePath, std::string& outResult)
{
	std::ifstream in(filePath, std::ios::in | std::ios::binary);
	if (in.is_open())
	{
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		if (size > 0)
		{
			outResult.clear();
			outResult.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&outResult[0], size);
			return true;
		}
		else
		{
			ECSE_LOG_ERROR(LogCore, "{} is empty", filePath);
		}
	}
	else
	{
		ECSE_LOG_ERROR(LogCore, "Could not open file {}", filePath);
	}

	return false;
}

bool FileSystem::tryReadFile(const std::filesystem::path& filePath, std::string& outResult)
{
	return tryReadFile(filePath.string(), outResult);
}

bool FileSystem::tryGetRootPath(std::filesystem::path& outPath)
{
	std::error_code errorCode;
	auto path = std::filesystem::current_path(errorCode);
	if (errorCode.value() != 0)
	{
		return false;
	}

	outPath = path.parent_path().string();
	return true;
}

bool FileSystem::tryGetEnginePath(std::filesystem::path& outPath)
{
	if (tryGetRootPath(outPath))
	{
		// todo #1: we probably need to expose a config for this
		outPath += "/Engine";
		return true;
	}
	return false;
}

std::vector<std::filesystem::path> FileSystem::getAllFilesWithExtension(const std::filesystem::path& directoryPath, const std::string& extension)
{
	namespace fs = std::filesystem;

	std::vector<fs::path> paths;
	if (fs::exists(directoryPath) && fs::is_directory(directoryPath))
	{
		for (const auto& entry : fs::directory_iterator(directoryPath))
		{
			if (fs::is_regular_file(entry) && entry.path().extension() == extension)
			{
				paths.push_back(entry.path());
			}
		}
	}
	return paths;
}