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
        // @brief �t�@�C���ꗗ�擾
        // folder : �t�H���_�̐�΃p�X����͂Ƃ��� 
        // �� : "D:\\Users\\Pictures\\"
        inline std::vector<std::string> GetFileListInDirectory(const char* directory_path, const char* file_name_pattern)
        {
            std::vector<std::string> fileList;

#ifdef _WIN32
            // �錾
            HANDLE hFind;
            WIN32_FIND_DATA fd;

            // �t�@�C���T��
            std::string search_path_pattern = std::string(directory_path) + "/" + file_name_pattern;
            hFind = FindFirstFile(search_path_pattern.c_str(), &fd);

            // �������s
            if (hFind == INVALID_HANDLE_VALUE) {
                std::cout << "�t�@�C���ꗗ���擾�ł��܂���ł���" << std::endl;
                throw std::runtime_error("file not found");
            }

            // �t�@�C���������X�g�Ɋi�[���邽�߂̃��[�v
            do {
                // �t�H���_�͏���
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
                {
                    //�t�@�C���������X�g�Ɋi�[
                    char *file = fd.cFileName;
                    std::string str = file;
                    fileList.push_back(str);
                }
            } while (FindNextFile(hFind, &fd)); //���̃t�@�C����T��

                                                // hFind�̃N���[�Y
            FindClose(hFind);
#else
            WSP_FATAL("Not implemented");
#endif
            return fileList;
        }
    }
}
