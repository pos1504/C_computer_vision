#ifndef FILE_IO_H
#define FILE_IO_H
#include<stdio.h>

// txt������ �ҷ����� �Լ�
int readDataFromFile(const char* filename, double* arr, int numRows, int numCols) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        perror("������ �� �� �����ϴ�.");
        return 0; // ���� �� 0 ��ȯ
    }

    // ���Ͽ��� �����͸� �о� 2���� �迭�� �����մϴ�.
    for (int i = 0; i < (numRows * numCols); i++) {
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
int save_2d_array_to_file(const char* filename, double* arr, int numRows, int numCols) {
    FILE* file;
    fopen_s(&file, filename, "w");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�.\n");
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