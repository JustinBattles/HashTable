#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SS 200000
#define TBLSZ 1009

char strSpace[SS];
char *f = strSpace;  //points to first free space in the array

struct Box {
  char *word;
  int  count;
  struct Box *next;
};

typedef struct Box box;

box *table[TBLSZ];  //initialized to 0

long hash(char *word) {
  long ans = 0;
  while (*word)
    ans = (ans<<1) ^ *word++;
  return ans%TBLSZ;
}

int myToupper(int c) {
  int d = 'A'-'a';
  if (c>='a' && c<= 'z') {
     c += d;
  }
  return c;
}

//returns 0 at EOF; 1 if it gets a word
//the word will be stored at address f in strSpace
int getWord(FILE *fp) {
  while (1) {
    int c = fgetc(fp);
    if (c==EOF) return 0;
    if ( (c>='A' && c<='Z') || (c>='a'  && c<='z') ) {
      ungetc(c, fp);
      fscanf(fp, "%[A-Za-z]", f);
      return 1;
    }
  }
}

//returns 0 if the word is NOT in the list
//returns 1 otherwise and increases the count of box by 1
//f points to the new word, index is the place the new word 
//would be found in the table.
int search(char *f, int index) {
  box *curr = table[index];
  while (curr) {
     if (strcmp(f, curr->word)==0) {
       curr->count++;
       return 1;
     }
     curr = curr->next;
  }
  return 0;
}

int main() {
  FILE *fp = fopen("/u1/junk/shakespeare.txt", "r");
  int count=0;
  char *longest = strSpace;
  int maxlength = 3;
  
  while (getWord(fp)) {
      //convert to upper case
      char *f1 = f;
      while(*f1) {
        *f1 = myToupper(*f1);
        f1++;
      }
      //
      //if (count>20) break;
      //Check is the word in the table?
      int index = hash(f);
      if (search(f, index)==0) { //new word is not already in list
         count++;
         //put the word into the list
            //make new box and add it to list
            box *nb = malloc(sizeof(box));
            nb->word = f;
            nb->count = 1;
            nb->next = table[index];
            table[index] = nb;
            //f no longer points to free space; we need to up date f
            f = f1+1;
            if (f-strSpace>900000) printf("Running out of string space\n");
      }
  }
  printf("total number of distinct words is %d\n", count);
  printf("total amount of strSpace: %d\n", f-strSpace);
  //total number of words in the list containing THE
  int index = hash("THE");
  printf("THE hashes to %d\n", index);
  box *curr = table[index];
  count =0;
  while(curr) {
    count++;
    printf("%s\n", curr->word);
    curr = curr->next;
  }
  printf("the number of words in the list is %d\n", count);
  int i;
  int k16=0;  //starting with zero lists of size 16
  for(i=0; i<1009; i++) {
     //find the length of the list at index i.
     box *curr = table[i];
     int knt=0;
     while(curr) {
       knt++;
       curr = curr->next;
     }
     if (knt==16)
         k16++;
  }
  printf("number of lists of size 16= %d\n", k16);
  printf("the most frequent word\n");
  char *mostFreq = 0;
  int freq = 0;
  int largest = 0;
  int empty = 0;
  int length = 1;
  char *lword;
  for (int i = 0; i < 1009; i++) {
	  box *curr = table[i];
	  while (curr) {
		  if (curr->count > freq) {
			  mostFreq = curr->word;
			  freq = curr->count;
		  }
		  curr = curr->next;
	  }
  }
  
  for (int k = 0; k < 1009; k++) {
	  box *curr = table[k];
	  int streak = 0;
	  while (curr) {
		  if (curr->next != 0)
			  streak++;
		  if (streak > largest)
			  largest = streak;
		  curr = curr->next;
			 
	  }
  }
  
  for (int n = 0; n < 1009; n++) {
	  box *curr = table[n];
	  while (curr) {
		  if (curr->count == 0)
			  empty++;
		  curr = curr->next;
	  }
  }
  
    for (int m = 0; m < 1009; m++) {
	  box *curr = table[m];
	  int streak = 0;
	  while (curr) {
		  if (strlen(curr->word) > streak)
			  streak = strlen(curr->word);
		  if (streak > length) {
			  length = streak;
			  lword = curr->word;
		  }
		  curr = curr->next;
	  }
  }

  
	  printf("word %s frequency %d\n", mostFreq, freq);
	  printf("Largest list: %d\n", largest);
	  printf("Empty lists: %d\n", empty);
	  printf("Longest word %s: %d\n", lword, length);
  }
  









