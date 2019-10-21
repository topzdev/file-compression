#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define MAX 100
#define FILE_EXTENSION ".hufzip"
#define TEMP_FILE "temp.huftmp"

typedef struct bsnode {
   int data;
   int len;
   char bin[255];
   struct bsnode *leftChild;
   struct bsnode *rightChild;
} BSTREE;

int arr[MAX];
char huffcode_bin[255][50];

BSTREE *bsr;
FILE *retrieve_file, *temp_file, *decompress_file;

//decleration of functions
void set_to_blank();
void decompress_data(char[]);
const char * dec_to_bin(int);
void insert_binary_tree();
void insert(int, const char[]);
int search(const char[]);


int main(){
    char filePath[50];
    printf("Enter the filepath to decompress here: ");
    scanf("%s", &filePath);
    decompress_data(filePath);
    return 0;
}

void decompress_data(char file_path[]){
    set_to_blank();
    int ch, i;
    char org_path[255], bin[255], char_arr[255], total_char, temp;
    strcpy(org_path,file_path);

    printf("\nFinding the given path...");
    retrieve_file = fopen(strcat(org_path,FILE_EXTENSION), "rb");
    temp_file = fopen(TEMP_FILE, "wb");
    decompress_file = fopen(file_path,"wb");


    if (retrieve_file == NULL){
        printf("\nFile path not found...");
    }
    else{
        printf("\nFile path sucessfully found...");
        for(i = 0; i < 256; i++){
            char_arr[i] = 0;
         }


        printf("\nDecompressing the file...");
        total_char = fgetc(retrieve_file);
        for(i= 0; i<total_char; i++){
            ch = fgetc(retrieve_file);
            if(ch != 13){
               char_arr[ch] = 1;
            }
        }

        for(i = 0; i < 256; i++){
           if(char_arr[i] != 0){
                fscanf(retrieve_file, "%s ",huffcode_bin[i]);
           }
        }

        insert_binary_tree();

        while(!feof(retrieve_file)){

            ch = fgetc(retrieve_file);

            if(ch != -1){
                fprintf(temp_file,"%s",dec_to_bin(ch));
            }
        }

        fclose(temp_file);



        printf("\nThe file is extracting the compressed data...");
        int n_ch = 0,  ret = -1;
        char temps[255];
        temp_file = fopen(TEMP_FILE, "r");
        i = 0;
        while(!feof(temp_file)){
            if(ret == -1){

                n_ch = fgetc(temp_file);
                temps[i] = (char)(n_ch);
                i++;
                temps[i] = '\0';
                ret = search(temps);
            }
            else{
                i = 0;
                fputc(ret, decompress_file);
                ret = -1;
                strcpy(temps, "");
            }
        }
        printf("\nThe file successfully decompressed...");
    }

    fclose(temp_file);
    fclose(retrieve_file);
    fclose(decompress_file);
    remove(TEMP_FILE);
}

void set_to_blank(){
    int i;
    for(i = 0; i<255; i++){
        strcpy(huffcode_bin[i], "");
    }
}

const char * dec_to_bin(int value){
    int i;
    static char bits[8];
    for (int bit = 8; bit; --bit) {
       i = (value & (1 << (bit - 1)) ? '1' : '0');
       bits[8 - bit] = (char)(i);
    }
    bits[8] = '\0';
    return bits;
}

void insert_binary_tree(){
    int i;
    BSTREE *temp;

    for(i = 0; i < 255; i++){
       if(strcmp(huffcode_bin[i], "") != 0){
            insert(i, huffcode_bin[i]);
       }
    }
}

void insert(int x, const char bin[]) {
   BSTREE *newNode = (BSTREE*) malloc(sizeof(BSTREE));
   BSTREE *current;
   BSTREE *parent;

   newNode->data = x;
   newNode->len = strlen(bin);
   strcpy(newNode->bin, bin);
   newNode->leftChild = NULL;
   newNode->rightChild = NULL;
   //if BSTREE is empty

   if(bsr == NULL) {
      bsr = newNode;
   } else {
      current = bsr;
      parent = NULL;

      while(1) {
         parent = current;

         //go to left of the BSTREE
         if(x < parent->data) {
            current = current->leftChild;

            //insert to the left
            if(current == NULL) {
               parent->leftChild = newNode;
               return;
            }
         }  //go to right of the BSTREE
         else {
            current = current->rightChild;

            //insert to the right
            if(current == NULL) {
               parent->rightChild = newNode;
               return;
            }
         }
      }
   }
}

int search(const char x[]) {
   BSTREE *current = bsr;
   int x_len = strlen(x);
  // printf("\nsearch is %s", x);

   while(current->len != x && strcmp(current->bin, x) != 0) {
    //   printf("\nhello there %s %s %c", current->bin, x, current->data);
      if(current != NULL)

      //go to left BSTREE
      if(current->len > x || strcmp(current->bin, x) == 0) {
      //if(strcmp(current->bin, x) > 0) {
         current = current->leftChild;
      }
      //else go to right BSTREE
      else {
         current = current->rightChild;
      }

      //not found
      if(current == NULL) {
         return -1;
      }
   }
   //printf("\ntobe return %d", current->data);
   return current->data;
}

