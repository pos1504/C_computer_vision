#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include<stdio.h>
#include<stdlib.h>
// 2���� ��ǥ�κ��� 1���� �ε����� ����ϴ� �Լ�
int getIndex(int row, int col, int numCols) {
    return row * numCols + col;
}
// 2���� zero padding�� �����ϰ� 1���� �е��� �迭�� ��ȯ�ϴ� �Լ�
double* pad2DArray_zero(double* arr2D, int numRows, int numCols, int paddingSize) {
    // �е� ó���� �迭�� ��� �� �� ���
    int paddedRows = numRows + 2 * paddingSize;
    int paddedCols = numCols + 2 * paddingSize;

    // Allocate memory for the padded array
    double* paddedArray = malloc(paddedRows * paddedCols * sizeof(double));
    if (paddedArray == NULL) {
        printf("�����Ҵ翡 �����߽��ϴ�.");
        return -1;
    }
    // Copy the elements from the original array with zero padding
    for (int i = 0; i < paddedRows; i++) {
        for (int j = 0; j < paddedCols; j++) {
            if (i < paddingSize || i >= numRows + paddingSize ||
                j < paddingSize || j >= numCols + paddingSize) {
                // ���� �迭 ���� ���̸� 0���� ����
                paddedArray[getIndex(i, j, paddedCols)] = 0.0;
            }
            else {
                // �� �ܿ��� ���� �迭�� �� ����
                paddedArray[getIndex(i, j, paddedCols)] = arr2D[getIndex(i - paddingSize, j - paddingSize, numCols)];
            }
        }
    }

    return paddedArray;
}

double* pad2DArray_symmetric(double* arr2D, int numRows, int numCols, int paddingSize) {
    // ��Ī �е��� ������ �迭�� ��� �� ���� ���
    int paddedRows = numRows + 2 * paddingSize;
    int paddedCols = numCols + 2 * paddingSize;

    // ��Ī �е��� �迭�� ���� �޸� �Ҵ�
    double* paddedArray = malloc(paddedRows * paddedCols * sizeof(double));
    if (paddedArray == NULL) {
        printf("�޸� �Ҵ翡 �����߽��ϴ�.");
        return NULL;
    }

    // ��Ī �е����� ���� �迭�� ���ҵ��� �����մϴ�.
    for (int i = 0; i < paddedRows; i++) {
        for (int j = 0; j < paddedCols; j++) {
            int sourceRow = i - paddingSize;
            int sourceCol = j - paddingSize;

            // �࿡ ���� ��Ī �е� ó��
            if (sourceRow < 0) {
                sourceRow = -sourceRow - 1;
            }
            else if (sourceRow >= numRows) {
                sourceRow = 2 * numRows - sourceRow - 1;
            }

            // ���� ���� ��Ī �е� ó��
            if (sourceCol < 0) {
                sourceCol = -sourceCol - 1;
            }
            else if (sourceCol >= numCols) {
                sourceCol = 2 * numCols - sourceCol - 1;
            }

            // ���� �迭�κ��� ���Ҹ� ��Ī �е��� �迭�� �����մϴ�.
            paddedArray[getIndex(i, j, paddedCols)] = arr2D[getIndex(sourceRow, sourceCol, numCols)];
        }
    }

    return paddedArray;
}

// ���͸� �����Ͽ� ��������� �����ϴ� �Լ�
void convolution(double* input, double* filter, double* output, int inputRows, int inputCols, int filterSize) {
    int outputRows = inputRows - filterSize + 1;
    int outputCols = inputCols - filterSize + 1;

    for (int i = 0; i < outputRows; i++) {
        for (int j = 0; j < outputCols; j++) {
            double sum = 0.0;
            for (int k = 0; k < filterSize; k++) {
                for (int l = 0; l < filterSize; l++) {
                    // ������� ��� ����
                    sum += input[getIndex(i + k, j + l, inputCols)] * filter[getIndex(k, l, filterSize)];
                }
            }
            output[getIndex(i, j, outputCols)] = sum;

            // �ݿø� ���� �� ���� üũ
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
