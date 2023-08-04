#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include<stdio.h>
#include<stdlib.h>
// 2차원 좌표로부터 1차원 인덱스를 계산하는 함수
int getIndex(int row, int col, int numCols) {
    return row * numCols + col;
}
// 2차원 zero padding을 수행하고 1차원 패딩된 배열을 반환하는 함수
double* pad2DArray_zero(double* arr2D, int numRows, int numCols, int paddingSize) {
    // 패딩 처리된 배열의 행과 열 수 계산
    int paddedRows = numRows + 2 * paddingSize;
    int paddedCols = numCols + 2 * paddingSize;

    // Allocate memory for the padded array
    double* paddedArray = malloc(paddedRows * paddedCols * sizeof(double));
    if (paddedArray == NULL) {
        printf("동적할당에 실패했습니다.");
        return -1;
    }
    // Copy the elements from the original array with zero padding
    for (int i = 0; i < paddedRows; i++) {
        for (int j = 0; j < paddedCols; j++) {
            if (i < paddingSize || i >= numRows + paddingSize ||
                j < paddingSize || j >= numCols + paddingSize) {
                // 원래 배열 범위 밖이면 0으로 설정
                paddedArray[getIndex(i, j, paddedCols)] = 0.0;
            }
            else {
                // 그 외에는 원래 배열의 값 복사
                paddedArray[getIndex(i, j, paddedCols)] = arr2D[getIndex(i - paddingSize, j - paddingSize, numCols)];
            }
        }
    }

    return paddedArray;
}

double* pad2DArray_symmetric(double* arr2D, int numRows, int numCols, int paddingSize) {
    // 대칭 패딩을 적용한 배열의 행과 열 개수 계산
    int paddedRows = numRows + 2 * paddingSize;
    int paddedCols = numCols + 2 * paddingSize;

    // 대칭 패딩된 배열을 위한 메모리 할당
    double* paddedArray = malloc(paddedRows * paddedCols * sizeof(double));
    if (paddedArray == NULL) {
        printf("메모리 할당에 실패했습니다.");
        return NULL;
    }

    // 대칭 패딩으로 원래 배열의 원소들을 복사합니다.
    for (int i = 0; i < paddedRows; i++) {
        for (int j = 0; j < paddedCols; j++) {
            int sourceRow = i - paddingSize;
            int sourceCol = j - paddingSize;

            // 행에 대한 대칭 패딩 처리
            if (sourceRow < 0) {
                sourceRow = -sourceRow - 1;
            }
            else if (sourceRow >= numRows) {
                sourceRow = 2 * numRows - sourceRow - 1;
            }

            // 열에 대한 대칭 패딩 처리
            if (sourceCol < 0) {
                sourceCol = -sourceCol - 1;
            }
            else if (sourceCol >= numCols) {
                sourceCol = 2 * numCols - sourceCol - 1;
            }

            // 원래 배열로부터 원소를 대칭 패딩된 배열로 복사합니다.
            paddedArray[getIndex(i, j, paddedCols)] = arr2D[getIndex(sourceRow, sourceCol, numCols)];
        }
    }

    return paddedArray;
}

// 필터를 적용하여 컨볼루션을 수행하는 함수
void convolution(double* input, double* filter, double* output, int inputRows, int inputCols, int filterSize) {
    int outputRows = inputRows - filterSize + 1;
    int outputCols = inputCols - filterSize + 1;

    for (int i = 0; i < outputRows; i++) {
        for (int j = 0; j < outputCols; j++) {
            double sum = 0.0;
            for (int k = 0; k < filterSize; k++) {
                for (int l = 0; l < filterSize; l++) {
                    // 컨볼루션 계산 수행
                    sum += input[getIndex(i + k, j + l, inputCols)] * filter[getIndex(k, l, filterSize)];
                }
            }
            output[getIndex(i, j, outputCols)] = sum;

            // 반올림 수행 및 범위 체크
            output[getIndex(i, j, outputCols)] = (int)output[getIndex(i, j, outputCols)] + 0.5;
            if (output[getIndex(i, j, outputCols)] < 0) {
                output[getIndex(i, j, outputCols)] = 0;
            }
            if (output[getIndex(i, j, outputCols)] > 255) {
                output[getIndex(i, j, outputCols)] = 255;
            }
        }
    }
}

#endif
