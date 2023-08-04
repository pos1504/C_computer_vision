#include <stdio.h>
#include <stdlib.h>

// 2차원 좌표로부터 1차원 인덱스를 계산하는 함수
int getIndex(int row, int col, int numCols) {
    return row * numCols + col;
}
// 2차원 zero padding을 수행하고 1차원 패딩된 배열을 반환하는 함수
double* pad2DArray(double* arr2D, int numRows, int numCols, int paddingSize) {
    // 패딩 처리된 배열의 행과 열 수 계산
    int paddedRows = numRows + 2 * paddingSize;
    int paddedCols = numCols + 2 * paddingSize;

    // Allocate memory for the padded array
    double* paddedArray = malloc(paddedRows * paddedCols * sizeof(double));

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
            output[getIndex(i, j, outputCols)] =(int)output[getIndex(i, j, outputCols)]+ 0.5;
            if (output[getIndex(i, j, outputCols)] < 0) {
                output[getIndex(i, j, outputCols)] = 0;
            }
            if (output[getIndex(i, j, outputCols)] > 255) {
                output[getIndex(i, j, outputCols)] = 255;
            }
        }
    }
}
// txt파일을 불러오는 함수
int readDataFromFile(const char* filename, double* arr ,int numRows ,int numCols ) {
 FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return 0; // 실패 시 0 반환
    }

    // 파일에서 데이터를 읽어 2차원 배열에 저장합니다.
    for (int i = 0; i < (numRows*numCols); i++) {
            if (fscanf_s(file, "%lf", &arr[i]) != 1) {
                fprintf(stderr, "파일에서 데이터를 읽을 수 없습니다.");
                fclose(file);
                return 0; // 실패 시 0 반환
            }
        
    }

    // 파일을 닫습니다.
    fclose(file);

    return 1; // 성공 시 1 반환
}
//txt 파일을 저장하는 함수
int save_2d_array_to_file(const char* filename, double* arr ,int numRows ,int numCols) {
    FILE* file;
    fopen_s(&file, filename, "w");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 0;
    }
    for (int i = 0; i < numRows*numCols; i++) {
       
            fprintf(file, "%d ",(int)arr[i]);
        
        fprintf(file, "\n");
    }

    fclose(file);

    return 1;
}


int main() {
    // 예시 2차원 배열
    int numRows = 220;
    int numCols = 220;

    double arr_R[48400] ={0};
    double arr_G[48400] ={0};
    double arr_B[48400] ={0};
    
    //txt읽어오기
    readDataFromFile("image_R.txt",arr_R ,numRows , numCols );
    readDataFromFile("image_G.txt",arr_G ,numRows , numCols );
    readDataFromFile("image_B.txt",arr_B ,numRows , numCols );
    /* 
     for (int i = 0; i < 48400; i++)
     {
         
             printf("%lf ", arr_R[i]);
      
     }
 */
    // 패딩 크기
    int paddingSize = 1;
    
    // 2차원 배열을 1차원 패딩된 배열로 변환
    double* paddedArray_R = pad2DArray((double*)arr_R,  numRows,  numCols,  paddingSize);
    double* paddedArray_G = pad2DArray((double*)arr_G,  numRows,  numCols,  paddingSize);
    double* paddedArray_B = pad2DArray((double*)arr_B,  numRows,  numCols,  paddingSize);

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
    double* output_G = malloc(outputRows * outputCols * sizeof(double));
    double* output_B = malloc(outputRows * outputCols * sizeof(double));

    // 컨볼루션 수행
    convolution(paddedArray_R, (double*)lpfFilter, output_R, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_R, (double*)lpfFilter, output_G, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_R, (double*)lpfFilter, output_B, paddedRows, paddedCols, filterSize);

    // 컨볼루션 결과 출력
    /* 
     printf("컨볼루션 결과:\n");
    for (int i = 0; i < outputRows; i++) {
        for (int j = 0; j < outputCols; j++) {
            printf("%lf ", output_R[getIndex(i, j, outputCols)]);
        }
        printf("\n");
    }
 */
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
