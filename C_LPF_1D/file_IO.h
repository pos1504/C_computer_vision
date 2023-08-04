#ifndef FILE_IO_H
#define FILE_IO_H
#include<stdio.h>

// txt파일을 불러오는 함수
int readDataFromFile(const char* filename, double* arr, int numRows, int numCols) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return 0; // 실패 시 0 반환
    }

    // 파일에서 데이터를 읽어 2차원 배열에 저장합니다.
    for (int i = 0; i < (numRows * numCols); i++) {
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
int save_2d_array_to_file(const char* filename, double* arr, int numRows, int numCols) {
    FILE* file;
    fopen_s(&file, filename, "w");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 0;
    }
    for (int i = 0; i < numRows * numCols; i++) {

        fprintf(file, "%d ", (int)arr[i]);

        fprintf(file, "\n");
    }

    fclose(file);

    return 1;
}

#endif