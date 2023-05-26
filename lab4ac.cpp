#include <iostream>
#include <windows.h>


uintmax_t getDirectorySizeRecursive(const wchar_t* directoryPath)
{
    uintmax_t totalSize = 0;
    std::wstring searchPattern = std::wstring(directoryPath) + L"\\*";
    WIN32_FIND_DATAW findData;
    HANDLE findHandle = FindFirstFileW(searchPattern.c_str(), &findData);

    if (findHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error" << std::endl;
        return 0;
    }

    do
    {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            ULARGE_INTEGER fileSize;
            fileSize.LowPart = findData.nFileSizeLow;
            fileSize.HighPart = findData.nFileSizeHigh;
            totalSize += fileSize.QuadPart;
        }
        else if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0)
        {
            std::wstring subdirectoryPath = std::wstring(directoryPath) + L"\\" + findData.cFileName;
            totalSize += getDirectorySizeRecursive(subdirectoryPath.c_str());
        }
    } while (FindNextFileW(findHandle, &findData));

    FindClose(findHandle);

    return totalSize;
}

int main()
{
    const wchar_t* directoryPath = L"C:\\Users\\2022\\Лабораторные\\ПТ221";  

    uintmax_t totalSize = getDirectorySizeRecursive(directoryPath);
    std::cout << "Sum size: " << totalSize << " bytes" << std::endl;

    return 0;
}