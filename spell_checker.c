#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "hashset_pa4.c"

#define MAX_WORD_LEN 60
#define MAX_LINE_LEN 1000
#define REPLACE 1
#define SWAP 2
#define ERASE 4

void print_possible_correction(char* misspelled_word, int flags, hashset dict){
    int printed = 0;
    if(flags & REPLACE){
        for(int i = 0; misspelled_word[i] != '\0'; i++){
            char original = misspelled_word[i];
            for(char c = 'a'; c <= 'z'; c++){
                if(c != original){
                    misspelled_word[i] = c;
                    if(search(dict, misspelled_word)){
                        if(printed == 0) {
                            printed = 1;
                            printf("%s", misspelled_word);
                        }
                        else
                            printf(",%s", misspelled_word);
                    }
                }
            }
            misspelled_word[i] = original;
        }
        
    }
    if(flags & SWAP){
        for(int i = 0; misspelled_word[i+1] != '\0'; i++){
            char original1 = misspelled_word[i];
            char original2 = misspelled_word[i+1];
            misspelled_word[i] = original2;
            misspelled_word[i+1] = original1;
            
            if(search(dict, misspelled_word)){
                if(printed == 0) {
                    printed = 1;
                    printf("%s", misspelled_word);
                }
                else
                    printf(",%s", misspelled_word);
            }

            misspelled_word[i] = original1;
            misspelled_word[i+1] = original2;
        }
    }
    if(flags & ERASE){
        int len = strlen(misspelled_word);
        for(int i = 0; i < len; i++){
            char erased[MAX_WORD_LEN];
            strncpy(erased, misspelled_word, i);
            erased[i] = '\0';
            strcat(erased, misspelled_word + i + 1);

            if(search(dict, erased)){
                if(printed == 0) {
                    printed = 1;
                    printf("%s", erased);
                }
                else
                    printf(",%s", erased);
				break;
            }

        }
    }
    if(printed) {
        printf("\n");
    }
}

int main(int argc, char* argv[]){
	int flags = 0;
	char* dictionary_filename = "words.txt";
	hashset set = set_init();

	for(int i = 1; i < argc;i++){
		if(argv[i][0] != '-')
			exit(1);
		if(!strcmp(argv[i], "-d")){
			if(i+1 == argc)
				exit(3);
			dictionary_filename = argv[++i];
			continue;
		}
		for(int j = 1; j < strlen(argv[i]);j++){
			switch(argv[i][j]){
				case 'e': flags |= ERASE;break;
				case 's': flags |= SWAP; break;
				case 'r': flags |= REPLACE; break;
				default: exit(2);
			}
		}
	}

	FILE* words_file = fopen(dictionary_filename, "r");
	char buffer[MAX_WORD_LEN];
	while(fgets(buffer, MAX_WORD_LEN, words_file)){
		int len = strlen(buffer);
		if(buffer[len-1] == '\n')
			buffer[len-1] = '\0';
		insert(&set, buffer);
	}
	fclose(words_file);

    int line = 1, col = 1;
    char lines[MAX_LINE_LEN];
    while(fgets(lines, MAX_LINE_LEN, stdin)){
        if(line == 1)
            printf("word,Ln,Col,Possible Corrections\n");
        char* token = strtok(lines, " \t\n");
        while (token != NULL){
            char word[MAX_WORD_LEN];
			int len = strlen(token);

			if(isdigit(token[0])){

			}
			else{
				//takes out the non-letters
				int j = 0;
				for (int i = 0; i < len; i++) {
					if (isalpha(token[i]) || token[i] == '\'') {
						word[j++] = token[i];
					}
				}
				word[j] = '\0';

				if (!search(set, word)) {
					printf("%s,%d,%d,", word, line, col);
					print_possible_correction(word, flags, set);
				}
			}

            col += strlen(token) + 1;
            token = strtok(NULL, " \t\n");
        }

        line++;
        col = 1;
    }

    return 0;
}