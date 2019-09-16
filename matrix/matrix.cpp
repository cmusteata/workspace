#include <iostream>
#include "matrix.h"

std::vector<std::vector<int>> matrix_sum(const std::vector<std::vector<int>>& before)
{
    std::vector<std::vector<int>> after(before.size());
    for (size_t i = 0; i < before.size(); ++i)
    {
        after[i].resize(before[i].size());
        for (size_t j = 0; j < before[i].size(); ++j)
        {
            if (i > 0 && j > 0)
                after[i][j] = before[i][j] + after[i-1][j] + after[i][j-1] - after[i-1][j-1];
            else if (i > 0)
                after[i][j] = before[i][j] + after[i-1][j];
            else if (j > 0)
                after[i][j] = before[i][j] + after[i][j-1];
            else
                after[i][j] = before[i][j];
        }
    }

    return after;
}

std::vector<std::vector<int>> matrix_restore(const std::vector<std::vector<int>>& after)
{
    std::vector<std::vector<int>> before(after.size());
    for (size_t i = 0; i < after.size(); ++i)
    {
        before[i].resize(after[i].size());
        for (size_t j = 0; j < after[i].size(); ++j)
        {
            if (i > 0 && j > 0)
                before[i][j] = after[i][j] - after[i-1][j] - after[i][j-1] + after[i-1][j-1];
            else if (i > 0)
                before[i][j] = after[i][j] - after[i-1][j];
            else if (j > 0)
                before[i][j] = after[i][j] - after[i][j-1];
            else
                before[i][j] = after[i][j];
        }
    }

    return before;
}

