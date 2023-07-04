/*
 * Created by vitriol1744 on 19.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Ustar.hpp"

#include "Utility/Math.hpp"
#include "VirtualFileSystem/DevTmpFs/DevTmpFs.hpp"

template <typename T>
inline static T parseOctNumber(const char* str, size_t len)
    requires std::is_trivial_v<T>
{
    T value = 0;
    while (*str && len > 0)
    {
        value = value * 8 + (*str++ - '0');
        --len;
    }

    return value;
}

namespace Ustar
{
    bool Validate(uintptr_t address)
    {
        return strncmp(reinterpret_cast<FileHeader*>(address)->signature, MAGIC,
                       MAGIC_LENGTH)
            == 0;
    }

    void Load(uintptr_t address)
    {
        auto current = reinterpret_cast<FileHeader*>(address);
        auto getNextFile
            = [](FileHeader* current, size_t fileSize) -> FileHeader*
        {
            uintptr_t nextFile = reinterpret_cast<uintptr_t>(current) + 512
                               + Math::AlignUp(fileSize, 512);

            return reinterpret_cast<FileHeader*>(nextFile);
        };

        while (strncmp(current->signature, MAGIC, 6) == 0)
        {
            std::string_view filename(current->filename);
            std::string_view linkName(current->linkName);

            mode_t           mode
                = parseOctNumber<mode_t>(current->mode, sizeof(current->mode));
            size_t size = parseOctNumber<size_t>(current->fileSize,
                                                 sizeof(current->fileSize));

            if (filename == "./" || filename == ".")
            {
                current = getNextFile(current, size);
                continue;
            }

            INode* node = nullptr;
            switch (current->type)
            {
                case FILE_TYPE_NORMAL:
                case FILE_TYPE_NORMAL_:
                    node = VirtualFileSystem::CreateNode(
                        nullptr, filename, mode | S_IFREG, INodeType::eRegular);

                    if (!node)
                    {
                        LogError(
                            "USTAR: Failed to create regular file!, path: "
                            "'{}'",
                            filename.data());
                        LogInfo("file created");
                        break;
                    }
                    else if (node->Write(
                                 reinterpret_cast<uint8_t*>(
                                     reinterpret_cast<uintptr_t>(current)
                                     + 512),
                                 0, size)
                             != ssize_t(size))
                        LogError(
                            "USTAR: Could not write to regular file! path: "
                            "'{}'",
                            filename.data());
                    break;
                case FILE_TYPE_HARD_LINK:
                    LogError("USTAR: Failed to create hardlink: '{}' -> '{}'",
                             filename.data(), linkName.data());
                    break;
                case FILE_TYPE_SYMLINK:
                    LogError("USTAR: Failed to create Symlink: '{}' -> '{}'",
                             filename.data(), linkName.data());
                    break;
                case FILE_TYPE_CHARACTER_DEVICE:
                {
                    uint32_t deviceMajor = parseOctNumber<uint32_t>(
                        current->deviceMajor, sizeof(current->deviceMajor));
                    uint32_t deviceMinor = parseOctNumber<uint32_t>(
                        current->deviceMinor, sizeof(current->deviceMinor));
                    LogError(
                        "USTAR: Failed to create character device: '{}' "
                        "({}:{})",
                        filename.data(), deviceMajor, deviceMinor);
                    ToDo();
                }
                break;
                case FILE_TYPE_BLOCK_DEVICE: ToDo();
                case FILE_TYPE_DIRECTORY:
                    node = VirtualFileSystem::CreateNode(nullptr, filename,
                                                         mode | S_IFDIR,
                                                         INodeType::eDirectory);
                    if (!node)
                        LogError(
                            "USTAR: Failed to create a directory! path: '{}'",
                            filename.data());
                    break;
                case FILE_TYPE_FIFO: ToDo() break;
                default: break;
            }

            current = getNextFile(current, size);
        }
    }
} // namespace Ustar