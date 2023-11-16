#include <stdio.h>

int main() {
    FILE *sourceFile, *destinationFile;
    char buffer[1024]; // Độ dài của buffer có thể được điều chỉnh

    // Mở tệp nguồn để đọc
    sourceFile = fopen("test.txt", "rb");
    if (sourceFile == NULL) {
        perror("Lỗi khi mở tệp nguồn");
        return 1;
    }

    // Mở tệp đích để viết
    destinationFile = fopen("test1.txt", "ab");
    if (destinationFile == NULL) {
        perror("Lỗi khi mở tệp đích");
        fclose(sourceFile); // Đóng tệp nguồn trước khi thoát
        return 1;
    }

    // Sao chép nội dung từ tệp nguồn vào tệp đích
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    // Đóng cả hai tệp
    fclose(sourceFile);
    fclose(destinationFile);

    printf("Sao chép thành công.\n");
    return 0;
}

