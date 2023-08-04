#include <stdio.h>
#include <stdlib.h>
#include "file_IO.h"
#include "convolution.h"
/* �迭�� ũ�Ⱑ 1161764����Ʈ ��� ������ ũ�� 1MB = 1000000����Ʈ ���� ũ�Ƿ� ������ �߻� 
��Ŀ���� ���ÿ���޸𸮸� �ø��ų�
 �迭�� �������� �������ָ�ȴ�.    
*/
double arr_R[48400] = { 0 }; 
double arr_G[48400] = { 0 };
double arr_B[48400] = { 0 };

int main() {
    // ���� 2���� �迭
    int numRows = 220;
    int numCols = 220;

    
    
    //txt�о����
    readDataFromFile("image_R.txt",arr_R ,numRows , numCols );
    readDataFromFile("image_G.txt",arr_G ,numRows , numCols );
    readDataFromFile("image_B.txt",arr_B ,numRows , numCols );
    
     for (int i = 0; i < 48400; i++)
     {
         
             printf("%lf ", arr_R[i]);
      
     }
 
    // �е� ũ��
    int paddingSize = 1;
    
    // 2���� �迭�� 1���� �е��� �迭�� ��ȯ
    double* paddedArray_R = pad2DArray((double*)arr_R,  numRows,  numCols,  paddingSize);
    double* paddedArray_G = pad2DArray((double*)arr_G,  numRows,  numCols,  paddingSize);
    double* paddedArray_B = pad2DArray((double*)arr_B,  numRows,  numCols,  paddingSize);
    // double* paddedArray = pad2DArray((double*)arr2D, numRows, numCols, paddingSize);

    // �е� ó���� �迭 ���
    int paddedRows = numRows + 2 * paddingSize;
    int paddedCols = numCols + 2 * paddingSize;
 /*     for (int i = 0; i < paddedRows; i++) {
        for (int j = 0; j < paddedCols; j++) {
            printf("%d ", (int)paddedArray_R[getIndex(i, j, paddedCols)]);
        }
        printf("\n");
    } */
 
    // LPF(���� ��� ����) ����
    int filterSize = 3;
    double lpfFilter[3][3] = {
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
    };

    // ������� ����� ������ �迭�� ���� �޸� �Ҵ�
    int outputRows = numRows;
    int outputCols = numCols;
    double* output_R = malloc(outputRows * outputCols * sizeof(double));
    if (output_R == NULL) {
        printf("�����Ҵ翡 �����߽��ϴ�.");
        return -1;
    }
    double* output_G = malloc(outputRows * outputCols * sizeof(double));
    if (output_G == NULL) {
        printf("�����Ҵ翡 �����߽��ϴ�.");
        return -1;
    }

    double* output_B = malloc(outputRows * outputCols * sizeof(double));
    if (output_B == NULL) {
        printf("�����Ҵ翡 �����߽��ϴ�.");
        return -1;
    }

    // ������� ����
    convolution(paddedArray_R, (double*)lpfFilter, output_R, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_G, (double*)lpfFilter, output_G, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_B, (double*)lpfFilter, output_B, paddedRows, paddedCols, filterSize);

    // ������� ��� ���
     printf("������� ���:\n");
    for (int i = 0; i < outputRows; i++) {
        for (int j = 0; j < outputCols; j++) {
            printf("%lf ", output_R[getIndex(i, j, outputCols)]);
        }
        printf("\n");
    }

save_2d_array_to_file("output_R.txt",output_R,numRows,  numCols);
save_2d_array_to_file("output_G.txt",output_G,numRows,  numCols);
save_2d_array_to_file("output_B.txt",output_B,numRows,  numCols);


    // �Ҵ�� �޸� ����
    free(paddedArray_R);
    free(output_R);
    free(output_G);
    free(output_B);

    return 0;
}
