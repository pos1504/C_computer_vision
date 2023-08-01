#include <stdio.h>
#include <stdlib.h>

#define ROWS 220
#define COLS 220
#define ROWS_F 3
#define COLS_F 3
#define SIZE 3
void readDataFromeFile(const char* filename, int arr[][COLS]);
// ������� �Լ� ����

double convolution_2D(int input[][COLS], double filtermask[][COLS_F], double output[][COLS]);



int main() {
    int img_R[ROWS][COLS]; 
    int img_G[ROWS][COLS]; 
    int img_B[ROWS][COLS]; 
    // �̹��� RGB �ҷ�����
    readDataFromFile("image_R.txt", img_R);
    readDataFromFile("image_G.txt", img_G);
    readDataFromFile("image_B.txt", img_B);

    

    double output[ROWS][COLS] = { 0 };
    
 
    /*���͸���ũ LPF �����Ҵ�
    double** filtermask;
    filtermask = (double**)malloc(sizeof(double*) * masksize);
    for (int i = 0; i < masksize; i++) {
        filtermask[i] = (double*)malloc(sizeof(double) * masksize);
    }
    
    for (int i = 0; i < masksize; i++) {
        for (int j = 0; j < masksize; j++) {
            filtermask[i][j] = 1 / (masksize * masksize);
        }
    }
    */
    // ���͸���ũ LPF
    double filtermask[ROWS_F][COLS_F] = {0}; 
    double size_f = ROWS_F * COLS_F;
    size_f = 1 / size_f;
        for (int i = 0; i < ROWS_F; i++) {
            for (int j = 0; j < COLS_F; j++) {
            filtermask[i][j] =  size_f;
            }
        }
        
           /*  LPF �迭 üũ
           for (int i = 0; i < ROWS_F; i++)
            {
                for (int j = 0; j < COLS_F; j++)
                {
                    printf("%lf ", filtermask[i][j]);
                }
            }
            */
      

    //�̹��� ��� ��
    int row = sizeof(img_R) / sizeof(img_R[0]);
    int col = sizeof(img_R[0]) / sizeof(int);
    
    // zero-padding
    int padding_row = (ROWS_F - 1) / 2;
    int padding_col = (COLS_F - 1) / 2;
    int padding_row_size = ROWS + (2 * padding_row);
    int padding_col_size = COLS + (2 * padding_col);

    printf("padding_row : %d \n", padding_row);
    printf("padding_col : %d \n", padding_col);
    printf("padding_row_size : %d \n", padding_row_size);
    printf("padding_col_size : %d \n", padding_col_size);
    
         int** zero_padding_array = malloc(sizeof(int*) * padding_row_size);
    for (int i = 0; i < padding_row_size; i++) {
        zero_padding_array[i] = malloc(sizeof(int) * padding_col_size);
    }

    for (int i = 0; i < padding_row_size; i++) {
        for (int j = 0; j < padding_col_size; j++) {
            zero_padding_array[i][j] = 0;
        }
    }


    int padding_row_length = padding_row_size - padding_row;
    int padding_col_length = padding_col_size - padding_col;

    for (int i = padding_row; i < padding_row_size - padding_row; i++) {
        for (int j = padding_col; j < padding_col_size - padding_col; j++) {
            zero_padding_array[i][j] = img_R[i-padding_row][j-padding_col];
        }
    }
    /*
    for (int i = 0; i < padding_row_size; i++)
    {
        for (int j = 0; j < padding_col_size; j++) {
            printf("%d ", zero_padding_array[i][j]);
        }
        printf("\n");
    }
    */
   

    // find center position of kernel (half of kernel size)
    

    for (int i = 0; i < ROWS; i++)              // rows
    {
        for (int j = 0; j < COLS; j++)          // columns
        {
            for (int u = 0; u < ROWS_F; u++)     // kernel rows
            {
    

                for (int v = 0; v < COLS_F; v++) // kernel columns
                {
                

                    // index of input signal, used for checking boundary
                   

                    // ignore input samples which are out of bound
                    //if (ii >= 0 && ii < ROWS && jj >= 0 && jj < COLS)
                        output[i][j] = output[i][j]+ zero_padding_array[i+u][j+v] * filtermask[u][v];
                }
            }
            
            output[i][j] = (int)(output[i][j] + 0.5); // �ݿø�
            if (output[i][j] < 0) output[i][j] = 0;
            if (output[i][j] > 255) output[i][j] = 255;
        }
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++) {
            printf("%d  ", (int)output[i][j]);
        }
        printf("\n");
    }
  

    
    //�޸� ����
    for (int i = 0; i < padding_row_size; i++) {
        free(zero_padding_array[i]);
    }
    free(zero_padding_array);



    return 0;
    


    /*if (readDataFromFile("image_R.txt", img_G)) {
        // �迭�� ����� �����͸� Ȯ���մϴ�.
        printf("2���� �迭�� ������:\n");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                printf("%d ", img_R[i][j]);
            }
            printf("\n");
        }
    }
    */
}




// ������ �ҷ�����
int readDataFromFile(const char* filename, int arr[][COLS]) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        perror("������ �� �� �����ϴ�.");
        return 0; // ���� �� 0 ��ȯ
    }

    // ���Ͽ��� �����͸� �о� 2���� �迭�� �����մϴ�.
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (fscanf_s(file, "%d", &arr[i][j]) != 1) {
                fprintf(stderr, "���Ͽ��� �����͸� ���� �� �����ϴ�.");
                fclose(file);
                return 0; // ���� �� 0 ��ȯ
            }
        }
    }

    // ������ �ݽ��ϴ�.
    fclose(file);

    return 1; // ���� �� 1 ��ȯ
}
// ������� �Լ� ����


double convolution_2D(int padded_input[][COLS], double filtermask[][COLS_F], double output[][COLS]) {

    // find center position of kernel (half of kernel size)
    int kCenterX = COLS_F / 2;
    int kCenterY = ROWS_F / 2;

    for (int i = 0; i < ROWS; ++i)              // rows
    {
        for (int j = 0; j < COLS; ++j)          // columns
        {
            for (int m = 0; m < ROWS_F; ++m)     // kernel rows
            {
                int mm = ROWS_F - 1 - m;      // row index

                for (int n = 0; n < COLS_F; ++n) // kernel columns
                {
                    int nn = COLS_F - 1 - n;  // column index

                    // index of input signal, used for checking boundary
                    int ii = i + (m - kCenterY);
                    int jj = j + (n - kCenterX);

                    // ignore input samples which are out of bound
                    if (ii >= 0 && ii < ROWS && jj >= 0 && jj < COLS)
                        output[i][j] += padded_input[ii][jj] * filtermask[m][n];
                }
            }
        }
    }
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++) {
            printf("%lf  ", output[i][j]);
        }
        printf("\n");
    }
}

