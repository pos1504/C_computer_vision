#include <stdio.h>
#include <stdlib.h>
#include "file_IO.h"
#include "convolution.h"

/* 배열이 1161764바이트보다 크다 컴파일러링커의 스택메모리는1MB = 1000000바이트이므로
    링커의 스택메모리를 늘리거나 
    배열은 힙영역에 전역변수로 선언해야한다. 
*/

double arr_R[48400] = { 0 }; // R 채널 이미지 배열
double arr_G[48400] = { 0 }; // G 채널 이미지 배열
double arr_B[48400] = { 0 }; // B 채널 이미지 배열

int main() {
    // 이미지의 2차원 크기
    int numRows = 220;
    int numCols = 220;

    // txt 파일에서 데이터 읽기
    readDataFromFile("image_R.txt", arr_R, numRows, numCols);
    readDataFromFile("image_G.txt", arr_G, numRows, numCols);
    readDataFromFile("image_B.txt", arr_B, numRows, numCols);

    // 이미지 데이터 출력
    /*
    for (int i = 0; i < 48400; i++) {
        printf("%lf ", arr_R[i]);
    }
    */
    // 패딩 처리를 위한 크기
    int paddingSize = 1;

    // 2차원 이미지를 1차원 배열로 변환하여 패딩 처리
    double* paddedArray_R = pad2DArray_symmetric((double*)arr_R, numRows, numCols, paddingSize);
    double* paddedArray_G = pad2DArray_symmetric((double*)arr_G, numRows, numCols, paddingSize);
    double* paddedArray_B = pad2DArray_symmetric((double*)arr_B, numRows, numCols, paddingSize);

 


    // 패딩 처리된 이미지의 행과 열 크기
    int paddedRows = numRows + 2 * paddingSize;
    int paddedCols = numCols + 2 * paddingSize;

    printf("필터링 결과:\n");
    for (int i = 0; i < paddedRows; i++) {
        for (int j = 0; j < paddedCols; j++) {
            printf("%d ", (int)paddedArray_R[getIndex(i, j, paddedCols)]);
        }
        printf("\n");
    }


    // LPF (Low Pass Filter) 필터
    int filterSize = 3;
    double lpfFilter[9] = {
        1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
        1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
        1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0
    };

    // 출력 이미지 배열 할당
    int outputRows = numRows;
    int outputCols = numCols;
    double* output_R = malloc(outputRows * outputCols * sizeof(double));
    if (output_R == NULL) {
        printf("메모리 할당에 실패했습니다.");
        return -1;
    }
    double* output_G = malloc(outputRows * outputCols * sizeof(double));
    if (output_G == NULL) {
        printf("메모리 할당에 실패했습니다.");
        return -1;
    }
    double* output_B = malloc(outputRows * outputCols * sizeof(double));
    if (output_B == NULL) {
        printf("메모리 할당에 실패했습니다.");
        return -1;
    }

    // 이미지 컨볼루션 (필터링)
    convolution(paddedArray_R, (double*)lpfFilter, output_R, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_G, (double*)lpfFilter, output_G, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_B, (double*)lpfFilter, output_B, paddedRows, paddedCols, filterSize);

    // 결과 이미지 출력
    /*
    printf("필터링 결과:\n");
    for (int i = 0; i < outputRows; i++) {
        for (int j = 0; j < outputCols; j++) {
            printf("%lf ", output_R[getIndex(i, j, outputCols)]);
        }
        printf("\n");
    }
    */
    // 결과 이미지를 파일로 저장
    save_2d_array_to_file("output_R.txt", output_R, numRows, numCols);
    save_2d_array_to_file("output_G.txt", output_G, numRows, numCols);
    save_2d_array_to_file("output_B.txt", output_B, numRows, numCols);

    // 메모리 해제
    free(paddedArray_R);
    free(output_R);
    free(output_G);
    free(output_B);

    return 0;
}
