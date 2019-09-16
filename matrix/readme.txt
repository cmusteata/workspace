Considering a matrix A(m,n) with m rows and n columns.
We define the sum matrix S(m,n) with the following property.
The value of each S[x][y] cell is the sum of all the elements
in the A(x+1,y+1) matrix, where x = [0, m) and y = [0, n).

for (int x = 0; x < m; ++x)
{
    for (int y = 0; y < n; ++y)
    {
        int sum = 0;
        for (int i = 0; i < x+1; ++i)
            for (int j = 0; j < y+1; ++j)
                sum += A[i][j];
        S[x][y] = sum;
    }
}

Being given the sum-matrix S(m,n) you have to calculate the initial matrix A(m,n).
Time complexity O(m*n).
