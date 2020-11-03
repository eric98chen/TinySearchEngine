/* querier.c --- querier part of tse
 * 
 * 
 * Author: Eric Chen
 * Created: Sun Nov  1 11:42:00 2020 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include "pageio.h" 
#include "webpage.h"
#include "queue.h"
#include "hash.h"
#include "indexio.h"

#define MAX 200



/* checks for non-alphabetic characters
 *   - if non-alphabetic character found, returns false
 *   - otherwise, converts all characters to lowercase, and return true
 */
bool NormalizeWord(char *word) {

	int i, n = strlen(word);
	char c;
	
	for (i=0; i<n; i++) {
		c = word[i];
		if ((isalpha(c) == 0) && (isspace(c) == 0)){ //returns false if non-alphabetic character AND not white space
			//printf("returning false\n");
			return false;
		}
		word[i] = tolower(c); //converts all alphabetic characters to lowercase
	}
	//printf("returning true\n"); 
	return true;
}


/* fn() compares words[i] with indexed words
 * returns bool
 */
bool fn(void* elementp, const void* word) {
	wc_t *ep = (wc_t*)elementp;
	char *wp = (char*)word;
	if (strcmp(ep->word,wp) == 0)  // true if the word is a match
		return true;
	else
		return false;
}


void rank_fn(void *p){
	char *unique;
	int count;
	
	unique = p->word;
	count = p->count
	
	printf("rank: 

}




/* look in queue for element with matching doc ID
 */
bool QueueSearchFn(void* elementp, const void* docID) { //requires (void* elementp, const void* keyp)
	counters_t *doc = (counters_t*)elementp;
	int *id = (int*)docID;
	return (doc->id == *id); //if match return true
}


int main (void) {

	char input[200]; //to store unparsed user input
	char *words[10]; //array to store input (max input = 10 words)
	
	char delimits[] = " \t\n"; //set delimiters as space and tab
	int i,j,wc; //to store and print words from words array
	
	hashtable_t *hp;
	wc_t *wc_found;
	counters_t *doc;
	int id = 1;
	
	printf(" > ");	

	//note: use fgets, not scanf, in order to read entire line 
	while (fgets(input, MAX, stdin) != NULL) { //loops until user enters EOF (ctrl+d)
	
		if (NormalizeWord(input)) { //input can only contain alphabetic-characters and white space
    		
			wc=0; //rewrite words array for each new input 
			words[wc] = strtok(input,delimits); //strtok() splits input according to delimits and returns next token
			
			while (words[wc] != NULL) { //stores all input words into words array
				//printf("%s ",words[i]); 
				wc++;
				words[wc] = strtok(NULL,delimits); //continue splitting input until strktok returns NULL (no more tokens)
			}
			
			for(j = 0; j<wc; j++) { //print words in words array
				printf("%s ", words[j]);
			}
			printf("\n");

			//*** STEP 2 ***
			hp = indexload("indexForQuery");
			
			for(i=0; i<wc; i++) {
				
				if((strcmp(words[i],"and")!=0) && (strcmp(words[i],"or")!=0) && (strlen(words[i])>=3)){
					
					if ((wc_found = hsearch(hp, fn, words[i], strlen(words[i]))) != NULL) {
						
						doc = qsearch(wc_found->qp, QueueSearchFn, &id);
						printf("%s:%d ", wc_found->word, doc->count);
						
					
					}
					
			
				}
			}
			
		
		} // end of valid query search
		else printf("[invalid query]"); //reject queries containing non-alphabetic/non-whitespace characters
			
		printf("\n > ");
	}
		
	printf("\n"); //add new line after user terminates with ctrl+d
	return(0);
	
}

