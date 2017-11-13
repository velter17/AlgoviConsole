#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include "termcolor/termcolor.hpp"

namespace {

#define LUBORDER '+'
#define RUBORDER '+'
#define LBBORDER '+'
#define RBBORDER '+'
#define HORBORDER '-'
#define VERBORDER '|'
#define BORDERBOTTOM '-'
#define BORDERRIGHT '|'
#define BORDERLEFT '|'
#define BORDERUP '-'
#define PLUSBORDER '+'

#define SPACE ' '

} // anonymous namespace

namespace SOME_NAME {
namespace Utils {

enum class EAlign
{
    Left,
    Right,
    Center
};

class Cell
{
public: // methods
    Cell();
    Cell& setAlign(const EAlign& a);
    EAlign align() const;
    Cell& setColor(const termcolor::color_t& color);
    const termcolor::color_t& color() const;
    Cell& setData(const std::vector<std::string>& data);
    const std::vector<std::string>& data() const;
    Cell& setCollspan(std::size_t v);
    std::size_t collspan() const;
    std::size_t height() const;
    std::size_t width() const;

private:
    EAlign m_align;
    termcolor::color_t m_color;
    std::vector<std::string> m_data;
    std::size_t m_collspan;
};

using Table = std::vector<std::vector<Cell>>;

inline Cell::Cell()
    : m_align(EAlign::Left)
    , m_color(termcolor::white)
    , m_collspan(1)
{
}

inline Cell& Cell::setAlign(const EAlign& a)
{
    m_align = a;
    return *this;
}

inline EAlign Cell::align() const
{
    return m_align;
}

inline Cell& Cell::setColor(const termcolor::color_t& color)
{
    m_color = color;
    return *this;
}

inline const termcolor::color_t& Cell::color() const
{
    return m_color;
}

inline Cell& Cell::setData(const std::vector<std::string>& data)
{
    m_data = data;
    return *this;
}

inline const std::vector<std::string>& Cell::data() const
{
    return m_data;
}

inline Cell& Cell::setCollspan(std::size_t v)
{
    m_collspan = v;
    return *this;
}

inline std::size_t Cell::collspan() const
{
    return m_collspan;
}

inline std::size_t Cell::width() const
{
    std::size_t ret = 0;
    for (const auto& it : m_data)
    {
        ret = std::max(ret, it.length());
    }
    return ret;
}

inline std::size_t Cell::height() const
{
    return m_data.size();
}

inline std::vector<std::vector<int>> getCellsWidth(const std::vector<std::vector<Cell>>& table)
{
    std::vector<std::vector<int>> ret(table.size());
    for (std::size_t i = 0; i < table.size(); ++i)
    {
        ret[i].resize(table[i].size(), 0);
    }
    std::vector<std::size_t> ptr(table.size(), 0);
    std::vector<int> cells(table.size());
    int completed = 0;
    for (int cell = 1; completed < table.size(); ++cell)
    {
        int mx = 0;
        for (std::size_t i = 0; i < table.size(); ++i)
        {
            if (ptr[i] == table[i].size())
            {
                continue;
            }
            if (cells[i] + table[i][ptr[i]].collspan() == cell)
            {
                int needAdd = std::max<int>(0, table[i][ptr[i]].width() + 2 - ret[i][ptr[i]]);
                mx = std::max(mx, needAdd);
            }
        }
        for (std::size_t i = 0; i < table.size(); ++i)
        {
            if (ptr[i] == table[i].size())
            {
                continue;
            }
            ret[i][ptr[i]] += mx;
            if (cells[i] + table[i][ptr[i]].collspan() == cell)
            {
                cells[i] = cell;
                ++ptr[i];
                if (ptr[i] == ret[i].size())
                {
                    ++completed;
                }
            }
        }
    }

    for (std::size_t i = 0; i < ret.size(); ++i)
    {
        for (std::size_t j = 0; j < ret[i].size(); ++j)
        {
            ret[i][j] += table[i][j].collspan() - 1;
        }
    }

    return ret;
}

inline std::string getTable(
    const std::vector<std::vector<Cell>>& table,
    termcolor::color_t border_color = termcolor::white)
{
    std::stringstream str;
    str << border_color;
    std::vector<int> h(table.size());
    for (std::size_t i = 0; i < table.size(); ++i)
    {
        for (std::size_t j = 0; j < table[i].size(); ++j)
        {
            h[i] = std::max(h[i], int(table[i][j].height()));
        }
    }

    std::vector<std::vector<int>> cellswidth = getCellsWidth(table);
    /*for (int i = 0; i < cellswidth.size(); ++i)
    {
        for (int j = 0; j < cellswidth[i].size(); ++j)
        {
            std::cout << cellswidth[i][j] << " ";
        }
        std::cout << std::endl;
    }*/

    auto drawTop = [&str, &border_color](const std::vector<int>& w) {
        str << termcolor::colorize << border_color << LUBORDER;
        for (std::size_t i = 0; i < w.size(); ++i)
        {
            str << std::string(w[i], HORBORDER);
            if (i == w.size() - 1)
            {
                str << RUBORDER;
            }
            else
            {
                str << BORDERBOTTOM;
            }
        }
        str << termcolor::reset;
        str << std::endl;
    };

    auto drawMid = [&str, &border_color](const std::vector<int>& w, const std::vector<int>& nxt) {
        std::vector<int> pt{0};
        int sumw = 0, sumnxt = 0;
        for (int t : w)
        {
            sumw += t + 1;
            pt.push_back(sumw);
        }
        for (int t : nxt)
        {
            sumnxt += t + 1;
            pt.push_back(sumnxt);
        }
        std::sort(pt.begin(), pt.end());
        int cnt = std::unique(pt.begin(), pt.end()) - pt.begin();
        pt.resize(cnt);
        /*std::cout << "pt: ";
        for (int t : pt)
        {
            std::cout << t << " ";
        }
        std::cout << std::endl;*/
        str << termcolor::colorize << border_color << BORDERRIGHT;
        std::size_t ptrw = 0, ptrnxt = 0;
        sumw = 0, sumnxt = 0;
        for (std::size_t i = 1; i < pt.size(); ++i)
        {
            str << std::string(pt[i] - pt[i - 1] - 1, HORBORDER);
            if (i == pt.size() - 1)
            {
                str << BORDERLEFT;
            }
            else
            {
                if (sumw + w[ptrw] > pt[i])
                {
                    str << BORDERBOTTOM;
                    sumnxt += nxt[ptrnxt++];
                }
                else if (sumnxt + nxt[ptrnxt] > pt[i])
                {
                    str << BORDERUP;
                    sumw += w[ptrw++];
                }
                else
                {
                    sumw += w[ptrw++];
                    sumnxt += nxt[ptrnxt++];
                    str << PLUSBORDER;
                }
            }
        }
        str << termcolor::reset;
        str << std::endl;
    };

    auto drawBottom = [&str, &border_color](const std::vector<int>& w) {
        str << termcolor::colorize << border_color << LBBORDER;
        for (std::size_t i = 0; i < w.size(); ++i)
        {
            str << std::string(w[i], HORBORDER);
            if (i == w.size() - 1)
            {
                str << RBBORDER;
            }
            else
            {
                str << BORDERUP;
            }
        }
        str << termcolor::reset;
        str << std::endl;
    };

    auto drawCellPart = [&str](const Cell& cell, int needw, int idx) {
        std::stringstream to_append;
        to_append << " ";
        if (cell.data().size() > idx)
        {
            to_append << cell.data()[idx];
        }
        int spaces = needw - to_append.str().size() - 1;
        to_append << " ";

        str << termcolor::colorize << cell.color();
        if (cell.align() == EAlign::Left)
        {
            str << to_append.str() << std::string(spaces, SPACE);
        }
        else if (cell.align() == EAlign::Right)
        {
            str << std::string(spaces, SPACE) << to_append.str();
        }
        else
        {
            str << std::string(spaces / 2, SPACE) << to_append.str()
                << std::string(spaces - (spaces / 2), SPACE);
        }
        str << termcolor::reset;
    };

    drawTop(cellswidth.front());
    for (std::size_t i = 0; i < table.size(); ++i)
    {
        for (int idx = 0; idx < h[i]; ++idx)
        {
            str << termcolor::colorize << border_color << VERBORDER << termcolor::reset;
            for (std::size_t j = 0; j < table[i].size(); ++j)
            {
                drawCellPart(table[i][j], cellswidth[i][j], idx);
                str << termcolor::colorize << border_color << VERBORDER << termcolor::reset;
            }
            str << std::endl;
        }
        i == table.size() - 1 ? drawBottom(cellswidth.back())
                              : drawMid(cellswidth[i], cellswidth[i + 1]);
    }

    return str.str();
}

} // namespace Utils
} // namespace SOME_NAME

