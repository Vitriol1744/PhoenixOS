/*
 * Created by vitriol1744 on 02.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <cwalk.h>
#include <details/string_view>
#include <vector>

namespace Path
{
    inline bool IsAbsolute(std::string_view path) { return path[0] == '/'; }

    inline std::vector<std::string_view> SplitPath_(std::string_view path)
    {
        std::vector<std::string_view> parts;
        std::size_t                   startPos = path[0] == '/' ? 1 : 0;
        std::size_t foundPos = path.find_first_of('/', startPos);

        while (foundPos != std::string_view::npos
               && path.length() > foundPos + 1)
        {
            std::string_view part = path.substr(startPos, foundPos - startPos);
            parts.push_back(part);

            startPos = foundPos + 1;
            foundPos = path.find_first_of('/', startPos);
        }

        // Handle the last part after the last '/' delimiter
        std::string_view lastPart = path.substr(startPos);
        parts.push_back(lastPart);

        return parts;
    }

    inline std::vector<std::string_view> SplitPath(std::string_view path)
    {
        std::vector<std::string_view> segments;

        cwk_segment                   current;
        cwk_path_get_first_segment(path.data(), &current);
        do segments.push_back(std::string_view(current.begin, current.size));
        while (cwk_path_get_next_segment(&current));

        return segments;
    }
} // namespace Path

using PathView = std::string_view;