#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define MAX 100

typedef struct bsnode {
   int data;
   int len;
   char bin[255];
   struct bsnode *leftChild;
   struct bsnode *rightChild;
} BSTREE;

typedef struct node{
	char data;
	unsigned int freq;
	struct node *left, *right;
}TREE;

typedef struct nodeArr{
	unsigned int size;
	unsigned int capacity;
	struct node** array;
}TREE_ARR;

TREE *root;
TREE_ARR *collection;
BSTREE *bsr;
int search(const char x[]);

int arr[MAX];
char encoded[255][50];

void makeNull(){
    root = NULL;
}


int main(){
    makeNull();
    char filePath[50];
    printf("Enter the filepath to decompress here: ");
    scanf("%s", &filePath);
    decompress(filePath);
    return 0;
}

void setToBlank(){
    int i;
    for(i = 0; i<255; i++){
        strcpy(encoded[i], "");
    }
}

FILE *file, *retrieve, *retVar, *orig;

void decompress(char file_path[]){
    setToBlank();
    printf("%s", file_path);

    retrieve = fopen(strcat(file_path,".huf"), "rb");
    retVar = fopen(strcat(file_path,".var"), "rb");
    orig = fopen(strcat(file_path,".org"),"rb");
    if(retrieve != NULL && retVar != NULL){
        int ch;
        char bin[255];
        while(!feof(retVar)){
            fscanf(retVar,"%d %s ", &ch, bin);
            printf("%d %s ", ch, bin);
            strcpy(encoded[ch],bin);
        }
        showDecoded();
        printf("\nTHE DECODED VALUE IS ================================================\n");
        int i = 0, n_ch = 0,  ret = -1;
        char temp[255], sh;
        while(!feof(retrieve)){
            if(ret == -1){
                n_ch = fgetc(retrieve);
                //printf("\nletter retrieve %c", n_ch);
                temp[i] = (char)(n_ch);
                i++;
                temp[i] = '\0';
                ret = search(temp);
                //printf("\nvalue of ret is %c\n", ret);
            }else{
                i = 0;
                printf("%c", ret);
                fprintf(orig, "%c", (char)(ret));
                ret = -1;
                strcpy(temp, "");
            }


        }
    }
    getch();
    fclose(retrieve);
    fclose(retVar);
    fclose(orig);
    getch();
}


void showDecoded(){
    int i;
    BSTREE *temp;

    printf("\n\tChar\tBinary\n");
    for(i = 0; i < 255; i++){
       if(strcmp(encoded[i], "") != 0){
            printf("\t%c\t%s\n",i, encoded[i]);
            insert(i, encoded[i]);
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
   //printf("\nsearch is %s", x);

   while(current->len != x && strcmp(current->bin, x) != 0) {
       //printf("\nhello there %s %s %c", current->bin, x, current->data);
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

