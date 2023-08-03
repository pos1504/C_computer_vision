#include <stdio.h>
#include <stdlib.h>

#define ROWS 220
#define COLS 220
#define ROWS_F 3
#define COLS_F 3
#define SIZE 3
int readDataFromeFile(const char* filename, int arr[][COLS]);

void save_2d_array_to_file( const char* filename, double arr[][COLS] );

int main() {

    int img_R[ROWS][COLS];
    int img_G[ROWS][COLS];
    int img_B[ROWS][COLS];
    // 이미지 RGB 불러오기
    readDataFromFile("image_R.txt", img_R);
    readDataFromFile("image_G.txt", img_G);
    readDataFromFile("image_B.txt", img_B);



    double output_R[ROWS][COLS] = { 0 };
    double output_G[ROWS][COLS] = { 0 };
    double output_B[ROWS][COLS] = { 0 };

 
    // 필터마스크 LPF
    double filtermask[ROWS_F][COLS_F] = { 0 };
    double size_f = ROWS_F * COLS_F;
    size_f = 1 / size_f;
    for (int i = 0; i < ROWS_F; i++) {
        for (int j = 0; j < COLS_F; j++) {
            filtermask[i][j] = size_f;
        }
    }

    /*  LPF 배열 체크
    for (int i = 0; i < ROWS_F; i++)
     {
         for (int j = 0; j < COLS_F; j++)
         {
             printf("%lf ", filtermask[i][j]);
         }
     }
     */



    //zero_padding_인자
    const int padding_row = (ROWS_F - 1) / 2;
    const int padding_col = (COLS_F - 1) / 2;
    const int padding_row_size = ROWS + (2 * padding_row);
    const int padding_col_size = COLS + (2 * padding_col);


    int padding_row_length = padding_row_size - padding_row;
    int padding_col_length = padding_col_size - padding_col;
    //zero padding 인자 확인
    
    printf("padding_row : %d \n", padding_row);
    printf("padding_col : %d \n", padding_col);
    printf("padding_row_size : %d \n", padding_row_size);
    printf("padding_col_size : %d \n", padding_col_size);
    

    //zero_padding_R_calloc
    int** zero_padding_array_R = calloc(padding_row_size,sizeof(int*) );
    for (int i = 0; i < padding_row_size; i++) {
        zero_padding_array_R[i] = calloc(padding_col_size,sizeof(int));
    }
    
    //zero_padding_G_calloc
    int** zero_padding_array_G = calloc(padding_row_size, sizeof(int*));
    for (int i = 0; i < padding_row_size; i++) {
        zero_padding_array_G[i] = calloc(padding_col_size, sizeof(int));
    }
    //zero_padding_B_calloc
    int** zero_padding_array_B = calloc(padding_row_size, sizeof(int*));
    for (int i = 0; i < padding_row_size; i++) {
        zero_padding_array_B[i] = calloc(padding_col_size, sizeof(int));
    }
    
   
    //zero_padding_R
    for (int i = padding_row; i < padding_row_size - padding_row; i++) {
        for (int j = padding_col; j < padding_col_size - padding_col; j++) {
            zero_padding_array_R[i][j] = img_R[i - padding_row][j - padding_col];
        }
    }
    //zero_padding_G
    for (int i = padding_row; i < padding_row_size - padding_row; i++) {
        for (int j = padding_col; j < padding_col_size - padding_col; j++) {
            zero_padding_array_G[i][j] = img_G[i - padding_row][j - padding_col];
        }
    }
    //zero_padding_B
    for (int i = padding_row; i < padding_row_size - padding_row; i++) {
        for (int j = padding_col; j < padding_col_size - padding_col; j++) {
            zero_padding_array_B[i][j] = img_B[i - padding_row][j - padding_col];
        }
    }
    
    //제로패딩 출력확인
    /*
        for (int i = 0; i < padding_row_size; i++)
        {
            for (int j = 0; j < padding_col_size; j++) {
                printf("%d  ", (int)zero_padding_array_R[i][j]);
            }
            printf("\n");
        }
      */  

        //컨볼루션_R
        for (int i = 0; i < ROWS; i++)              // rows
        {
            for (int j = 0; j < COLS; j++)          // columns
            {
                for (int u = 0; u < ROWS_F; u++)     // kernel rows
                {
                    for (int v = 0; v < COLS_F; v++) // kernel columns
                    {
                        output_R[i][j] = output_R[i][j] + zero_padding_array_R[i + u][j + v] * filtermask[u][v];
                    }
                }
                //반올림
                output_R[i][j] = (int)(output_R[i][j] + 0.5);
                if (output_R[i][j] < 0) output_R[i][j] = 0;
                if (output_R[i][j] > 255) output_R[i][j] = 255;
            }
        }
        //컨볼루션_G
        for (int i = 0; i < ROWS; i++)              // rows
        {
            for (int j = 0; j < COLS; j++)          // columns
            {
                for (int u = 0; u < ROWS_F; u++)     // kernel rows
                {
                    for (int v = 0; v < COLS_F; v++) // kernel columns
                    {
                        output_G[i][j] = output_G[i][j] + zero_padding_array_G[i + u][j + v] * filtermask[u][v];
                    }
                }
                //반올림
                output_G[i][j] = (int)(output_G[i][j] + 0.5);
                if (output_G[i][j] < 0) output_G[i][j] = 0;
                if (output_G[i][j] > 255) output_G[i][j] = 255;
            }
        }
        //컨볼루션_B
        for (int i = 0; i < ROWS; i++)              // rows
        {
            for (int j = 0; j < COLS; j++)          // columns
            {
                for (int u = 0; u < ROWS_F; u++)     // kernel rows
                {
                    for (int v = 0; v < COLS_F; v++) // kernel columns
                    {
                        output_B[i][j] = output_B[i][j] + zero_padding_array_B[i + u][j + v] * filtermask[u][v];
                    }
                }
                //반올림
                output_B[i][j] = (int)(output_B[i][j] + 0.5);
                if (output_B[i][j] < 0) output_B[i][j] = 0;
                if (output_B[i][j] > 255) output_B[i][j] = 255;
            }
        }
 
        
        //output 출력확인
      /*
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++) {
                printf("%d  ", (int)output_R[i][j]);
            }
            printf("\n");
        }
      */
        save_2d_array_to_file ("output_R.txt",output_R );

        save_2d_array_to_file("output_G.txt",output_G );

        save_2d_array_to_file("output_B.txt",output_B);



        //메모리 해제_R
        for (int i = 0; i < padding_row_size; i++) {
            free(zero_padding_array_R[i]);
        }
        free(zero_padding_array_R);
        
        //메모리 해제_G
        for (int i = 0; i < padding_row_size; i++) {
            free(zero_padding_array_G[i]);
        }
        free(zero_padding_array_G);
        //메모리 해제_B
        for (int i = 0; i < padding_row_size; i++) {
            free(zero_padding_array_B[i]);
        }
        free(zero_padding_array_B);
       

        return 0;
    }
    




// 데이터 불러오기
int readDataFromFile(const char* filename, int arr[][COLS]) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return 0; // 실패 시 0 반환
    }

    // 파일에서 데이터를 읽어 2차원 배열에 저장합니다.
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (fscanf_s(file, "%d", &arr[i][j]) != 1) {
                fprintf(stderr, "파일에서 데이터를 읽을 수 없습니다.");
                fclose(file);
                return 0; // 실패 시 0 반환
            }
        }
    }

    // 파일을 닫습니다.
    fclose(file);

    return 1; // 성공 시 1 반환
}
//데이터 저장
void save_2d_array_to_file(const char* filename, double arr[][COLS]) {
    FILE* file;
    fopen_s(&file, filename, "w");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(file, "%d ",(int)arr[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    return 1;
}