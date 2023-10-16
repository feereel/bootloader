#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FFK 4
#define BOOT_SIZE 512
#define MAX_TEXT_LEN 255
#define MAX_KEY_LEN 8


ssize_t find_offset(FILE* file){
    char sym;
    char find_sym = (char)0xff;
    int offset = 0, k = 0;
    
    while(offset < BOOT_SIZE){
        sym = fgetc(file);
        if (sym == find_sym) k += 1;
        else k = 0;
        offset += 1;

        if(k == 4){
            fseek(file, -4, SEEK_CUR);
            return offset;
        }
    }
    fseek(file, 0, SEEK_SET);
    return -1;
}

void encrypt_text(char* text, int text_len, char* key, int key_len){
    for (size_t i = 0; i < text_len; i++){
        text[i] ^= key[i % key_len];
    }
}


int main(int argc, char* argv[]){
    if (argc < 4){
        printf("Usage: ./encode.exe BOOT TEXT KEY\n");
        return 0;
    }
    
    char* file_name = argv[1];
    char* text = argv[2];
    char* key = argv[3];

    int text_len = strlen(text);
    if (text_len > MAX_TEXT_LEN){
        perror("MAX_TEXT_LEN");
        return __LINE__;
    }

    int key_len = strlen(key);
    if (key_len != MAX_KEY_LEN){
        perror("MAX_KEY_LEN");
        return __LINE__;
    }

    FILE* file = fopen(file_name, "r+b");
    if (file == NULL){
        perror("fopen");
        return __LINE__;
    }
    if (find_offset(file) < 0){
        perror("fild_offset");
        return __LINE__;
    }

    encrypt_text(text, text_len, key, key_len);

    fwrite(&text_len, sizeof(char), 1, file);

    fwrite(text, sizeof(char), text_len, file);

    printf("Writed successfully!\n");

    fclose(file);
    return 0;
}
