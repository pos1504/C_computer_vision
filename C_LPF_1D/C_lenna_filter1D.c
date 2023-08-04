#include <stdio.h>
#include <stdlib.h>
#include "file_IO.h"
#include "convolution.h"
/* 배열의 크기가 1161764바이트 사용 스택의 크기 1MB = 1000000바이트 보다 크므로 오류가 발생 
링커에서 스택예약메모리를 늘리거나
 배열을 전역으로 선언해주면된다.    
*/
double arr_R[48400] = { 0 }; 
double arr_G[48400] = { 0 };
double arr_B[48400] = { 0 };

int main() {
    // 예시 2차원 배열
    int numRows = 220;
    int numCols = 220;

    
    
    //txt읽어오기
    readDataFromFile("image_R.txt",arr_R ,numRows , numCols );
    readDataFromFile("image_G.txt",arr_G ,numRows , numCols );
    readDataFromFile("image_B.txt",arr_B ,numRows , numCols );
    
     for (int i = 0; i < 48400; i++)
     {
         
             printf("%lf ", arr_R[i]);
      
     }
 
    // 패딩 크기
    int paddingSize = 1;
    
    // 2차원 배열을 1차원 패딩된 배열로 변환
    double* paddedArray_R = pad2DArray((double*)arr_R,  numRows,  numCols,  paddingSize);
    double* paddedArray_G = pad2DArray((double*)arr_G,  numRows,  numCols,  paddingSize);
    double* paddedArray_B = pad2DArray((double*)arr_B,  numRows,  numCols,  paddingSize);
    // double* paddedArray = pad2DArray((double*)arr2D, numRows, numCols, paddingSize);

    // 패딩 처리된 배열 출력
    int paddedRows = numRows + 2 * paddingSize;
    int paddedCols = numCols + 2 * paddingSize;
 /*     for (int i = 0; i < paddedRows; i++) {
        for (int j = 0; j < paddedCols; j++) {
            printf("%d ", (int)paddedArray_R[getIndex(i, j, paddedCols)]);
        }
        printf("\n");
    } */
 
    // LPF(저역 통과 필터) 필터
    int filterSize = 3;
    double lpfFilter[3][3] = {
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
    };

    // 컨볼루션 결과를 저장할 배열을 위한 메모리 할당
    int outputRows = numRows;
    int outputCols = numCols;
    double* output_R = malloc(outputRows * outputCols * sizeof(double));
    if (output_R == NULL) {
        printf("동적할당에 실패했습니다.");
        return -1;
    }
    double* output_G = malloc(outputRows * outputCols * sizeof(double));
    if (output_G == NULL) {
        printf("동적할당에 실패했습니다.");
        return -1;
    }

    double* output_B = malloc(outputRows * outputCols * sizeof(double));
    if (output_B == NULL) {
        printf("동적할당에 실패했습니다.");
        return -1;
    }

    // 컨볼루션 수행
    convolution(paddedArray_R, (double*)lpfFilter, output_R, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_R, (double*)lpfFilter, output_G, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_R, (double*)lpfFilter, output_B, paddedRows, paddedCols, filterSize);

    // 컨볼루션 결과 출력
     printf("컨볼루션 결과:\n");
    for (int i = 0; i < outputRows; i++) {
        for (int j = 0; j < outputCols; j++) {
            printf("%lf ", output_R[getIndex(i, j, outputCols)]);
        }
        printf("\n");
    }

save_2d_array_to_file("output_R.txt",output_R,numRows,  numCols);
save_2d_array_to_file("output_G.txt",output_G,numRows,  numCols);
save_2d_array_to_file("output_B.txt",output_B,numRows,  numCols);


    // 할당된 메모리 해제
    free(paddedArray_R);
    free(output_R);
    free(output_G);
    free(output_B);

    return 0;
}
