def task22(matrix, columns, rows):
    result = 0
    for i in range(rows):
        for j in range(columns):
            if matrix[i][j] >= -2 and matrix[i][j] <= 2:
                result += 1
                print(i, j)
    return result


matrixExample = [[j for j in range(7)] for i in range(5)]
print(task22(matrixExample, 7, 5))
