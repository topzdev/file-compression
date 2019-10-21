#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#define MAX 100
#define FILE_EXTENSION ".hufzip"
#define TEMP_FILE "temp.huftmp"
//For Binary Search Tree
typedef struct bsnode {
   int data;
   char bin[50];
   struct bsnode *leftChild;
   struct bsnode *rightChild;
} BSTREE;

//For Huffman Code Tree
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
char* search(int x);

int arr[MAX];
char huffcode_bin[255][50];

//File handling identifiest
FILE *file, *store, *temp_file, *orig_file;

//Functions prototype
TREE* newNode(const char, unsigned);
TREE_ARR* createMinHeap(unsigned);
TREE *extractDataMin(TREE_ARR*);
TREE *build_huffman_tree(char[], int[], int);
TREE_ARR *createAndBuildMinHeap(char[], int[], int);
void buildMinHeap(TREE_ARR*);
void get_huffcode(TREE*, int[], int);
void insertMinHeap(TREE_ARR*,TREE*);
int isSizeOne(TREE_ARR*);
void minHeapify(TREE_ARR*, int);
void insertMinHeap(TREE_ARR*,TREE*);
void buildMinHeap(TREE_ARR*);
void build_huffman(char[], int[], int);

//Utilies and compression function
void makeNull();
void set_to_blank();
void display_decoded();
void extract_data(char[]);
void compress_data(char[], int);
int bin_to_dec (int);
int char_to_bin(const char[]);
void convert_to_bits(const char[]);
//Binary Search Tree
void insert(int x, const char bin[]);

void makeNull(){
    root = NULL;
}

int main(){
    makeNull();
    char file_path[50];
    printf("Enter the file_path here: ");
    scanf("%s", &file_path);
    clock_t begin = clock();
    extract_data(file_path);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTotal time of compression is: %f seconds\n", time_spent);

    return 0;
}

void extract_data(char file_path[]){
    unsigned int ch[255], freqArr[255] , size = 0, idx = 0, i, temp;
    char str[2], charArr[255];
    float freq = 0, total = 0 ;

    file = fopen(file_path, "r");

    for(i = 0; i < 255; i++){
        ch[i] = 0;
    }

    if(file == NULL){
        printf("File not found\n");
        return;
    }else{

        while(!feof(file)){
            temp = fgetc(file);
            ch[temp]++;
            size++;
        }

    }

    for(i = 0; i < 255; i++){
        if(ch[i] != 0){
            freq = (float)ch[i]/size * 100;
            total += freq;

            freqArr[idx] = ch[i];
            charArr[idx] = i;
            idx++;
        }
    }
    printf("\nBuilding the huffman binary code...");
    build_huffman(charArr, freqArr, idx);
    fclose(file);
    printf("\nFile compressing...");
    compress_data(file_path, idx);
    printf("\nFile sucessfully compressed...");
}

void compress_data(char file_path[], int total_char){
    int i;
    char temp;
    file = fopen(file_path, "r");
    store = fopen(strcat(file_path,FILE_EXTENSION), "wb");
    temp_file = fopen(TEMP_FILE,"w");
    orig_file = fopen(file_path,"w");
    if(file == NULL){
        printf("File not found\n");
        return;
    }else{
        //savefirst the letter and binary

        fprintf(store,"%c",total_char);
        for(i = 0; i < 255; i++){
           if(strcmp(huffcode_bin[i], "") != 0){
                fputc(i,store);
                //fprintf(store,"%s ",huffcode_bin[i]);
           }
        }
        for(i = 0; i < 255; i++){
           if(strcmp(huffcode_bin[i], "") != 0){
                fprintf(store,"%s ",huffcode_bin[i]);
           }
        }
        fclose(store);

        while(!feof(file)){
          temp = fgetc(file);
          fprintf(temp_file,"%s",search(temp));

        }
        fclose(temp_file);
        convert_to_bits(file_path);

    }

    fclose(file);
    fclose(orig_file);
    remove(TEMP_FILE);

}

TREE* newNode(const char data, unsigned freq){
	TREE* temp = (TREE*)malloc(sizeof(TREE));
	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;
	return temp;
}

TREE_ARR* createMinHeap(unsigned capacity){
	TREE_ARR* minHeap = (TREE_ARR*)malloc(sizeof(TREE_ARR));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (TREE**)malloc(minHeap->capacity * sizeof(TREE*));
	return minHeap;
}

void build_huffman(char data[], int freq[], int size){
	TREE* root = build_huffman_tree(data, freq, size);

	int top = 0;
	set_to_blank();
	get_huffcode(root, arr, top);
	display_decoded();
}

TREE *build_huffman_tree(char data[], int freq[], int size){
	TREE *left, *right, *top;
	TREE_ARR* minHeap = createAndBuildMinHeap(data, freq, size);
	while (!isSizeOne(minHeap)) {
		left = extractDataMin(minHeap);
		right = extractDataMin(minHeap);
		top = newNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	return extractDataMin(minHeap);
}

void insertMinHeap(TREE_ARR* minHeap,TREE* minHeapNode){
	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = minHeapNode;
}

TREE *extractDataMin(TREE_ARR* minHeap){

	TREE* temp = minHeap->array[0];
	minHeap->array[0]
		= minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}

void convert_to_bits(const char file_name[]){
    int i;
    char binValue[8];
    store = fopen(file_name,"ab");
    temp_file = fopen(TEMP_FILE,"r");
    int temp;
    if(temp_file != NULL){
        while(!feof(temp_file)){
            for(i = 0; i<8; i++){
                temp =   fgetc(temp_file);
                if(temp == -1){
                    break;
                }
                binValue[i] = (char)temp;
            }
            binValue[8] = '\0';
            if(strcmp(binValue, "")!=0){
                fprintf(store,"%c",char_to_bin(binValue));
                strcpy(binValue, "");
            }
        }


    } else{
        printf("no file found");
    }
    fclose(temp_file);
    fclose(store);
}

int char_to_bin(const char bits_8[]){
    int i = 0, value=0;
    int tempBin;
    value = atoi(bits_8);
    tempBin = bin_to_dec (value);
    return tempBin;

}

int bin_to_dec (int n){
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

void swapMinHeapNode(TREE** a, TREE** b){

	TREE* t = *a;
	*a = *b;
	*b = t;
}

void minHeapify(TREE_ARR* minHeap, int idx){

	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq){
        smallest = left;
	}

	if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq){
        smallest = right;
	}

	if (smallest != idx) {
		swapMinHeapNode(&minHeap->array[smallest],&minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

int isSizeOne(TREE_ARR* minHeap){
	return (minHeap->size == 1);
}

TREE_ARR *createAndBuildMinHeap(char data[], int freq[], int size){

	TREE_ARR* minHeap = createMinHeap(size);

	for (int i = 0; i < size; ++i){
		minHeap->array[i] = newNode(data[i], freq[i]);
	}
	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}

void buildMinHeap(TREE_ARR* minHeap){

	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

int isLeaf(TREE* root){
	return !(root->left) && !(root->right);
}

void set_to_blank(){
    int i;
    for(i = 0; i<255; i++){
        strcpy(huffcode_bin[i], "");
    }
}

void get_huffcode(TREE* root, int arr[], int top){

    int i;
    char temp[50];

	if (root->left) {
		arr[top] = 0;
		get_huffcode(root->left, arr, top + 1);
	}

	if (root->right) {
		arr[top] = 1;
		get_huffcode(root->right, arr, top + 1);
	}

	if (isLeaf(root)) {
        for (i = 0; i < top; i++){
            temp[i] = arr[i]+'0';
        }
        temp[top] = '\0';
        strcpy(huffcode_bin[(int)root->data], temp);
	}
}

void display_decoded(){
    int i;
    BSTREE *temp;

    //printf("\n\tChar\tBinary\n");
    for(i = 0; i < 255; i++){
       if(strcmp(huffcode_bin[i], "") != 0){
            //printf("\t%c\t%s\n",i, huffcode_bin[i]);
            insert(i, huffcode_bin[i]);
       }
    }
}

void insert(int x, const char bin[]) {
   BSTREE *newNode = (BSTREE*) malloc(sizeof(BSTREE));
   BSTREE *current;
   BSTREE *parent;

   newNode->data = x;
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

char* search(int x) {
   BSTREE *current = bsr;

   while(current->data != x) {
      if(current != NULL)

      //go to left BSTREE
      if(current->data > x) {
         current = current->leftChild;
      }
      //else go to right BSTREE
      else {
         current = current->rightChild;
      }

      //not found
      if(current == NULL) {
         return NULL;
      }
   }

   return current->bin;
}
