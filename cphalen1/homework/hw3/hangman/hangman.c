#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_LENGTH 255
#define NUM_GUESSES 8

// This function picks a random word and returns it in buffer.
// It obtains it by reading /usr/share/dict/words *ignoring any word containing characters other // than A-Z or a-z*.
//
// Returns true if success, false on error.
bool pickword(char * buffer, size_t maxlength) {
  // Pulling dictionary words from the machine's dictionary
  FILE * f = fopen("/usr/share/dict/words", "r");

	// Checking to ensure that our file is not a NULL pointer
	if(!f) {
		return false;
	}

  int lines = 0, i;
  long double probability; // This number will get really qutie small, so we need it to be a long double
  bool copy;
  char s[maxlength];
  srand(time(NULL));

  while(fgets(s, maxlength, f)) {
    // This probability algorithm allows us to select a random line from the file without having to know the numbers of lines -- thus we can get away with reading the file only once
  	probability = 1.0/(++lines);
    // Each line has a (1/lines) probability of overwriting the other lines, thus the probabilities even out and every line is equally likely to be selected
  	if(probability >= ((double)rand() / (double)RAND_MAX)) {
  		copy = 1;
  		for(i = 0; i < strlen(s); i++){
  			if(!isalpha(s[i]) && !isblank(s[i]) && s[i] != '\n'){
          // If our word is not alphabetical we should not write it to the buffer, and it should not incremement the lines, as that would put off the probabilities
  				copy = 0;
  				--lines;
  				break;
  			} else if(isupper(s[i])) {
          // The word being returned should be entirely lowercase
  				s[i] = tolower(s[i]);
  			} else if(s[i] == '\n') {
          // We don't want to add extraneous characters, otherwise the strlen() funciton would no work correctly
          s[i] = '\0';
        }
  		}

  		if(copy == 1)
  			strncpy(buffer, s, maxlength);
  	}
  }

  return true;
}


// Show diagram;
// Use your best ASCII-art skills!
void showdiagram (unsigned int incorrect) {
  // We go line by line here, and depending on how many guesses we have remaining, certain lines will be printed while others are left blank

  printf("\n----");
  if(incorrect > 0)
    printf("\n|   |");
  else
    printf("\n|    ");

  if(incorrect > 1)
    printf("\n|   o");
  else
    printf("\n|    ");

  if(incorrect == 3)
    printf("\n|   |");
  else if(incorrect == 4)
    printf("\n|  -|");
  else if(incorrect > 4)
    printf("\n|  -|-");
  else
    printf("\n|    ");

  if(incorrect > 5)
    printf("\n|   |");
  else
    printf("\n|    ");

  if(incorrect == 7)
    printf("\n|    \\\n"); // This looks somewhat confusing, but really what we are doing is espcaing the '\' and then adding a new line
  else if(incorrect > 7)
    printf("\n|  / \\ \n");
  else
    printf("\n|     \n");
}


// This function should prompt (i.e. write: Your next guess? )
// and then read a string from the keyboard, *only accepting it if it consists out of a
// single character (followed by newline) of the set a-z (or A-Z)*
//
// Return a /lower case/ version of the character that was read.
char readnext() {
  printf("\nSelect a character guess: ");
  char currentGuess = getchar();
  // We what to read the characters until the user hits enter -- additionally we will only return the first character selected
  while('\n'!=getchar());
  if(!isalpha(currentGuess)) {
    // Here we can use recursion to make our lives a little easier if they enter to add non-alphabetical values, this what we can avoid while loops which mess with the getchar() function
    printf("--please select an alphabetical character--\n");
    return readnext();
  }

  return currentGuess;
}

int main (int argc, char ** args) {
    char answerWord[MAX_LENGTH];
    bool wordPicked = pickword(&answerWord[0],MAX_LENGTH);

	if(wordPicked == false) {
		printf("\n\n--An error occured selecting a word for the hangman game--\n\n");
		return EXIT_FAILURE;
	}

    static char guessedCharacters[NUM_GUESSES];
    // We create a constant that represents the length of the string so we can avoid calling the strlen() function over and over again
    const int wordLength = strlen(answerWord);
    // We make a whole string of variables here to keep track of the player
    int guessesRemaining = NUM_GUESSES, lettersGuessed = 0;
    char foundCharacters[wordLength], currentGuess;
    int i;

    // This is a string that we will print which shows which parts of the string have been guessed
    for(i = 0; i < wordLength; i++) {
      foundCharacters[i] = '_';
    }

    foundCharacters[wordLength] = '\0';

    while(lettersGuessed < wordLength && guessesRemaining > 0) {
    	printf("\nCurrently your word: %s (you have %i guesses remaining)", foundCharacters, guessesRemaining);
    	currentGuess = readnext();

      // If the string already contains our charater we will not decrement the number of guesses
      if(strchr(guessedCharacters, currentGuess) == false) {
        guessedCharacters[NUM_GUESSES - guessesRemaining] = currentGuess;
      } else {
        continue;
      }

      // Here we run through the word and add the elements that match our guess into the foundCharacters string
    	for(i = 0; i < wordLength; i++) {
    		if(answerWord[i] == guessedCharacters[NUM_GUESSES - guessesRemaining]) {
    			foundCharacters[i] = answerWord[i];
          		++lettersGuessed;
    		}
    	}

      // If the guess did not match any of the characters, we show the gallows and decrement the number of guesses remaining
      if(strchr(foundCharacters, currentGuess) == false) {
        showdiagram(NUM_GUESSES - (--guessesRemaining));
      }

      // These are more of the win/loss conditionals
      if(lettersGuessed >= wordLength) {
        printf("\n\n---Congratulations--- you have won! Your word is guessed entirely\n\n");
        printf("\nYour word: %s (you finished with %i guesses leftover)\n\n", foundCharacters, guessesRemaining);
        }
      else if(guessesRemaining <= 0)
        printf("\n\nSorry! It looks like you're all out of guesses! Better luck next time ;)\n\n");

    }

    return EXIT_SUCCESS;
}
