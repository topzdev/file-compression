#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define MAX 100

typedef struct bsnode {
   int data;
   char bin[50];
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
char* search(int x);

int arr[MAX];
char decoded[255][50];

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

void makeNull(){
    root = NULL;
}

int main(){
    makeNull();
    char filePath[50];
    printf("Enter the filepath here: ");
    scanf("%s", &filePath);
    extract(filePath);
    return 0;
}

FILE *file, *store, *var;

void extract(char file_path[]){
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
    printf("\tASC\tChar\tFreq\tCalc\n");
    for(i = 0; i < 255; i++){
        if(ch[i] != 0){
            if(i == 10){
               strcpy(str, "NL");
            }else if(i == 9){
                strcpy(str, "TB");
            }else if(i == 32){
                strcpy(str, "SP");
            }else{
                str[0] = i;
                str[1] = 32;
            }
            freq = (float)ch[i]/size * 100;
            total += freq;

            freqArr[idx] = ch[i];
            charArr[idx] = i;
            idx++;
            printf("\t%i\t%s\t%i\t%5.3f\n",i,str, ch[i], freq);
        }
    }

    for(i = 0; i < idx; i++){
        printf("\t'%c' = %d \n", charArr[i], freqArr[i]);
    }
    printf("\n\tTotal Frequency %.2f\n",total);
    HuffmanCodes(charArr, freqArr, idx);
    fclose(file);

    compress(file_path);
}


void compress(const char filePath[]){
    int i;
    char bin[255];
    char temp;
    file = fopen(filePath, "r");
    store = fopen(strcat(filePath,".huf"), "wb");
    var = fopen(strcat(filePath,".var"), "wb");
    if(file == NULL){
        printf("File not found\n");
        return;
    }else{
        //savefirst the letter and binary
        for(i = 0; i < 255; i++){
           if(strcmp(decoded[i], "") != 0){
                printf("%d %s \n",i, decoded[i]);
                fprintf(var, "%d %s ", i, decoded[i]);
           }
        }

        while(!feof(file)){
          temp = fgetc(file);
          if(temp != NULL){
            //strcpy(bin, search(temp));
              fprintf(store,"%s",search(temp));
              //printf("%s", search(temp));
              //printf("%c", temp);
          }
        }


    }
    getch();
     //printf("The encoded data: %s", encoded);
    fclose(var);
    fclose(file);
    fclose(store);


}

void printstringasbinary(char* s)
{
    char output[9];
    while (*s)
    {
        itoa(*s, output, 2);
        puts(output);
        ++s;
    }
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

TREE *extractMin(TREE_ARR* minHeap){

	TREE* temp = minHeap->array[0];
	minHeap->array[0]
		= minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
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

TREE *buildHuffmanTree(char data[], int freq[], int size){
	TREE *left, *right, *top;
	TREE_ARR* minHeap = createAndBuildMinHeap(data, freq, size);
	while (!isSizeOne(minHeap)) {
		left = extractMin(minHeap);
		right = extractMin(minHeap);
		top = newNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}
	return extractMin(minHeap);
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

void buildMinHeap(TREE_ARR* minHeap){

	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

int isLeaf(TREE* root){
	return !(root->left) && !(root->right);
}
void HuffmanCodes(const char data[], int freq[], int size){
	TREE* root = buildHuffmanTree(data, freq, size);

	int top = 0;
	setToBlank();
	printCodes(root, arr, top);
	showDecoded();
}
void setToBlank(){
    int i;
    for(i = 0; i<255; i++){
        strcpy(decoded[i], "");
    }
}
void printCodes(TREE* root, int arr[], int top){

    int i;
    char temp[50];

	if (root->left) {
		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}

	if (root->right) {
		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}

	if (isLeaf(root)) {
        for (i = 0; i < top; i++){
            temp[i] = arr[i]+'0';
        }
        temp[top] = '\0';
        strcpy(decoded[(int)root->data], temp);
	}
}

void showDecoded(){
    int i;
    BSTREE *temp;

    printf("\n\tChar\tBinary\n");
    for(i = 0; i < 255; i++){
       if(strcmp(decoded[i], "") != 0){
            printf("\t%c\t%s\n",i, decoded[i]);
            insert(i, decoded[i]);
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
