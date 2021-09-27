/* Assignment 2
 * COMP10002 Foundations of Algorithms, Semester 1, 2021
 
 * //algorithms are awesome!!!!!
 * Full Name: Ryan Goh Rui En      
 * Student Number: 1191761 
 * Date:   15/5/2021         
 */

/****** Include libraries ******/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

/****** Definitions of constants ******/
#define MAXALPHABET 23 //22+1(nullbyte)
#define MAXPOSTAG 25   //4*5+4(whitespace)+1(nullbyte)
#define MAXFORM 105    //4*20+4(whitespace)+1(nullbyte)
#define MAXDICTIONARY 100
#define MAXWORDLENGTH 26 //25+1(nullbyte)

/****** Type definitions ******/
typedef char word_t[MAXALPHABET]; 
typedef char pos_t[MAXPOSTAG];    
typedef char form_t[MAXFORM];    
typedef struct{
    word_t word;
    pos_t pos;
    form_t form;
} wordstruct_t;
typedef wordstruct_t wordpackagearray_t[MAXDICTIONARY];
typedef struct node node_t;
typedef char data_t[MAXWORDLENGTH];
struct node {
	data_t data;
	node_t *next;
};
typedef struct {
	node_t *head;
	node_t *foot;
} list_t; 

/****** Function prototypes ******/
void stage1(wordstruct_t* firstword);
void stage2(wordstruct_t firstword, int* numwords, wordpackagearray_t* dictionary);
void stage3(int numwords, wordpackagearray_t dictionary, list_t *list);
void stage4(int numwords, wordpackagearray_t dictionary, list_t *list);

//stage 1 function prototypes
wordstruct_t readpackage();
void scanchar(char *array);

//stage 2 function prototypes
float averagevariation(float numwords, wordpackagearray_t dictionary);
int adddictionary (wordpackagearray_t dictionary);

//stage 3 function prototypes
list_t *make_empty_list(void);
list_t *insert_at_foot(list_t *list, data_t value);
int binary_search(wordpackagearray_t dictionary, int numwords, char *target);
void printlist(list_t *list, wordpackagearray_t dictionary,int numwords);
void print_binarycheck(wordpackagearray_t dictionary,int numwords, char* string);

//stage 4 function prototypes
int findform(char *target, char *dictionaryform);

/****** Main Function ******/
int main (int argc, char *argv[]){
    //declarations
    int numwords=0;
    wordstruct_t firstword;
    wordpackagearray_t dictionary;
    list_t *list = make_empty_list();
    
    stage1(&firstword);
    stage2(firstword,&numwords, &dictionary);
    stage3(numwords, dictionary, list);
    stage4(numwords, dictionary, list);
    
    printf("\n");
    free(list);
    return 0;
}

/************************************************************** Stage 1 **************************************************************/
void stage1(wordstruct_t *firstword){
    char symbol;
    scanf("%c",&symbol);
    *firstword = readpackage();
    printf("==========================Stage 1==========================");
    printf("\nWord 0: %s", firstword->word);
    printf("\nPOS: %s", firstword->pos);
    printf("\nForm: %s", firstword->form);
}
/********************************************************* Stage 1 Functions ************************************************************/

//This function reads 3 lines of the input, assigning each line to the corresponding variable in the struct.
wordstruct_t readpackage(){
    wordstruct_t wordpackage;
    char readchar;
    
    //scan word
    scanf("%s\n",&(wordpackage.word[0]));
    
    //scan POS
    scanchar(&wordpackage.pos[0]);
    
    //scan form
    readchar=getchar();
    if(readchar=='#'){
        scanchar(&wordpackage.form[0]);
    }
    return wordpackage;
}

//this function scans in the entire line as a string
void scanchar(char *array){
    int i =0;
    char readchar;
    while((readchar=getchar())!='\n'){
        array[i]=readchar;
        i++;
    }
    array[i]='\0';
}

/************************************************************** Stage 2 **************************************************************/
void stage2(wordstruct_t firstword, int *numwords, wordpackagearray_t *dictionary){
    //add first word into dictionary
    strcpy(dictionary[0]->word,firstword.word);
    strcpy(dictionary[0]->pos,firstword.pos);
    strcpy(dictionary[0]->form,firstword.form);
    
    //add other words into dictionary
    *numwords=adddictionary(*dictionary);
    
    //print
    printf("\n==========================Stage 2==========================");
    printf("\nNumber of words: %d", *numwords);
    printf("\nAverage number of variation forms per word: %.2f", averagevariation(*numwords, *dictionary));
}
/********************************************************* Stage 2 Functions ************************************************************/

//this function calculates the number of variations and the average number of variations per word
float averagevariation(float numwords, wordpackagearray_t dictionary){
    int numvar=0;
    for(int j=0;j<numwords;j++){
        for(int i=0;i<MAXFORM;i++){
            if(dictionary[j].form[i]>='0'&& dictionary[j].form[i]<='9'){
                numvar++;
            }
        }
    }
    return (float)numvar/numwords;
}

//this function adds the rest of the input words into the dictionary and returns the number of words added
int adddictionary (wordpackagearray_t dictionary){
    int numwords=1;
    char symbol;
    wordstruct_t tempword;
    
    for(;numwords<=MAXDICTIONARY;numwords++){
        symbol = getchar();
        if (symbol == '$'){
            tempword = readpackage();
            strcpy(dictionary[numwords].word,tempword.word);
            strcpy(dictionary[numwords].pos,tempword.pos);
            strcpy(dictionary[numwords].form,tempword.form);
        }
        if (symbol == '*'){
            scanf("*********\n");
            break;
        }
    }
    return numwords;
}

/************************************************************** Stage 3 **************************************************************/
void stage3(int numwords, wordpackagearray_t dictionary, list_t *list){
    //declarations
    node_t* originalhead;
	char c, tempstring[26];
    int i=0;
    
    //insert sentence into linked list
    while((c=getchar())!=EOF){
        if(c!=' '&&c!='\n'){
            tempstring[i]=c;
            i++;
        }
        if(c==' '){
            tempstring[i]='\0';
            insert_at_foot(list, tempstring);
            i=0;
        }
    }
    tempstring[i]='\0';
    insert_at_foot(list, tempstring);   //insert last word in sentence
    printf("\n==========================Stage 3==========================");
    originalhead = list->head;
    printlist(list, dictionary,numwords);
    list->head = originalhead;
}

/********************************************************* Stage 3 Functions ************************************************************/
//the following function comes  from listops.c provided by the subject
list_t* make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

//the following function comes  from listops.c provided by the subject. It has been modified so that the value of each node stores a string instead.
list_t* insert_at_foot(list_t *list, data_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	strcpy(new->data,value);
	new->next = NULL;
	if (list->foot==NULL) {
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

//this function does a binary search on the dictionary to find the word and returns the position of the word
int binary_search(wordpackagearray_t dictionary, int numwords, char *target){
    int bottom= 0;
    int mid;
    int top = numwords - 1;
    while(bottom <= top){
        mid = (bottom + top)/2;
        if (strcmp(dictionary[mid].word, target) == 0){
            return mid;
        } else if (strcmp(dictionary[mid].word, target) > 0){
            top    = mid - 1;
        } else if (strcmp(dictionary[mid].word, target) < 0){
            bottom = mid + 1;
        }
    }
    return 0;
} 

//this function prints the contents of the linked list while using binary search to find the POS
void printlist(list_t* list, wordpackagearray_t dictionary, int numwords){
    while(list->head){
        printf("\n%-26s",list->head->data);
        print_binarycheck(dictionary,numwords, list->head->data);
        list->head = list->head->next;
        if (list->head==NULL) {
            list->foot = NULL;
        }
    }
}

//Takes in a string and searches the dictionary for it. If it is found, it will print the POS of the word. Else, print NOT_FOUND
void print_binarycheck(wordpackagearray_t dictionary,int numwords, char* string){
    if(!binary_search(dictionary, numwords, string)){
                    printf("NOT_FOUND");
                }
                else{
                    printf("%s",dictionary[binary_search(dictionary, numwords, string)].pos);
                }
}

/************************************************************** Stage 4 **************************************************************/
void stage4(int numwords, wordpackagearray_t dictionary, list_t *list){
    int check = 0;
    node_t *oldhead;
    printf("\n==========================Stage 4==========================");
    while(list->head){
        printf("\n%-26s",list->head->data);
        for(int i=0; i<numwords; i++){
            check = 0; //boolean
            //check if word in the sentence is already in root form
            if(!strcmp(dictionary[i].word, list->head->data)){
                printf("%-26s", dictionary[i].word);
                print_binarycheck(dictionary,numwords, dictionary[i].word);
                check = 1;
                break;  
            }
            //if not in root form, check if word in the sentence is in variation form
            if(strcmp(dictionary[i].word, list->head->data)){
                if(findform(list->head->data,dictionary[i].form)){
                    printf("%-26s", dictionary[i].word);
                    printf("%s", dictionary[i].pos);
                    check = 1;
                    break;
                } 
            }
         }
        // if word is not found in the either root form or variation forms
        if(check==0){
            printf("%-26s",list->head->data);
            print_binarycheck(dictionary,numwords, list->head->data);
            
        }
        oldhead = list->head;
        list->head = list->head->next;
        if (list->head==NULL) {
            list->foot = NULL;
        }
        free(oldhead);
    }
}

/********************************************************* Stage 4 Functions ************************************************************/
//this function loops through dictionary[i].form to find the targetted word. returns 1 if found. 
int findform(char *target, char *dictionaryform){
    char tempword[MAXWORDLENGTH];
    int tempwordlength=0;
    int i=0;
    for(; dictionaryform[i]!='\0'; i++){
        //building the string
        if(isalpha(dictionaryform[i])){
            tempword[tempwordlength]=dictionaryform[i];
            tempwordlength++;
        }
        //end of string 
        if(!isalpha(dictionaryform[i])&&(i)){
            tempword[tempwordlength]='\0';
            if(!strcmp(tempword,target)){
                return 1;
            }
            tempwordlength=0;
        }
    }
    //last string
    tempword[tempwordlength]='\0';
    if(!strcmp(tempword,target)){
        return 1;
    }
    return 0;
}


/*********************************************************  time complexity ************************************************************/
//algorithms are awesome!!!!!
/*
Given that:
s words in the sentence
d words in the dictionary
each dictionary word has up to f variation forms
each form has a maximum length of m characters.

Worst case: 
O(s*d*f*m)

Explaination:
For each word in the sentence, this program will search the through the dictionary to find it in the root form. This would take O(s*log(d)*m)
If it is not present, it would fine it in the variation forms. This would take O(s*d*f*m)
Assuming this is the worst case complexity, assume the word is found in the variation forms and since each string comparison takes O(m).
This would take O(s*log(d)*m+s*d*f*m) which can be simplified to O(s*d*f*m)
*/