#include <stdio.h>
#include <stdlib.h>

// 2���� ��ǥ�κ��� 1���� �ε����� ����ϴ� �Լ�
int getIndex(int row, int col, int numCols) {
    return row * numCols + col;
}
// 2���� zero padding�� �����ϰ� 1���� �е��� �迭�� ��ȯ�ϴ� �Լ�
double* pad2DArray(double* arr2D, int numRows, int numCols, int paddingSize) {
    // �е� ó���� �迭�� ��� �� �� ���
    int paddedRows = numRows + 2 * paddingSize;
    int paddedCols = numCols + 2 * paddingSize;

    // Allocate memory for the padded array
    double* paddedArray = malloc(paddedRows * paddedCols * sizeof(double));

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
// txt������ �ҷ����� �Լ�
int readDataFromFile(const char* filename, double* arr ,int numRows ,int numCols ) {
 FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        perror("������ �� �� �����ϴ�.");
        return 0; // ���� �� 0 ��ȯ
    }

    // ���Ͽ��� �����͸� �о� 2���� �迭�� �����մϴ�.
    for (int i = 0; i < (numRows*numCols); i++) {
            if (fscanf_s(file, "%lf", &arr[i]) != 1) {
                fprintf(stderr, "���Ͽ��� �����͸� ���� �� �����ϴ�.");
                fclose(file);
                return 0; // ���� �� 0 ��ȯ
            }
        
    }

    // ������ �ݽ��ϴ�.
    fclose(file);

    return 1; // ���� �� 1 ��ȯ
}
//txt ������ �����ϴ� �Լ�
int save_2d_array_to_file(const char* filename, double* arr ,int numRows ,int numCols) {
    FILE* file;
    fopen_s(&file, filename, "w");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�.\n");
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
    // ���� 2���� �迭
    int numRows = 220;
    int numCols = 220;

    double arr_R[48400] ={0};
    double arr_G[48400] ={0};
    double arr_B[48400] ={0};
    
    //txt�о����
    readDataFromFile("image_R.txt",arr_R ,numRows , numCols );
    readDataFromFile("image_G.txt",arr_G ,numRows , numCols );
    readDataFromFile("image_B.txt",arr_B ,numRows , numCols );
    /* 
     for (int i = 0; i < 48400; i++)
     {
         
             printf("%lf ", arr_R[i]);
      
     }
 */
    // �е� ũ��
    int paddingSize = 1;
    
    // 2���� �迭�� 1���� �е��� �迭�� ��ȯ
    double* paddedArray_R = pad2DArray((double*)arr_R,  numRows,  numCols,  paddingSize);
    double* paddedArray_G = pad2DArray((double*)arr_G,  numRows,  numCols,  paddingSize);
    double* paddedArray_B = pad2DArray((double*)arr_B,  numRows,  numCols,  paddingSize);

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
    double* output_G = malloc(outputRows * outputCols * sizeof(double));
    double* output_B = malloc(outputRows * outputCols * sizeof(double));

    // ������� ����
    convolution(paddedArray_R, (double*)lpfFilter, output_R, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_R, (double*)lpfFilter, output_G, paddedRows, paddedCols, filterSize);
    convolution(paddedArray_R, (double*)lpfFilter, output_B, paddedRows, paddedCols, filterSize);

    // ������� ��� ���
    /* 
     printf("������� ���:\n");
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


    // �Ҵ�� �޸� ����
    free(paddedArray_R);
    free(output_R);
    free(output_G);
    free(output_B);

    return 0;
}
