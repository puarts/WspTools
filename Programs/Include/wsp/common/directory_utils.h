#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "_define_commoncore.h"

#ifdef _WIN32
#include <Windows.h>
#endif


namespace wsp {
    namespace common {
        // @brief ファイル一覧取得
        // folder : フォルダの絶対パスを入力とする 
        // 例 : "D:\\Users\\Pictures\\"
        inline std::vector<std::string> GetFileListInDirectory(const char* directory_path, const char* file_name_pattern)
        {
            std::vector<std::string> fileList;

#ifdef _WIN32
            // 宣言
            HANDLE hFind;
            WIN32_FIND_DATA fd;

            // ファイル探索
            std::string search_path_pattern = std::string(directory_path) + "/" + file_name_pattern;
            hFind = FindFirstFile(search_path_pattern.c_str(), &fd);

            // 検索失敗
            if (hFind == INVALID_HANDLE_VALUE) {
                std::cout << "ファイル一覧を取得できませんでした" << std::endl;
                throw std::runtime_error("file not found");
            }

            // ファイル名をリストに格納するためのループ
            do {
                // フォルダは除く
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
                {
                    //ファイル名をリストに格納
                    char *file = fd.cFileName;
                    std::string str = file;
                    fileList.push_back(str);
                }
            } while (FindNextFile(hFind, &fd)); //次のファイルを探索

                                                // hFindのクローズ
            FindClose(hFind);
#else
            WSP_FATAL("Not implemented");
#endif
            return fileList;
        }
    }
}
