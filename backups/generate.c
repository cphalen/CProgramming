#include "board.h"

#define MINI 0
#define MAX 1

int generate(board_t * board, int step, player_t color, unsigned int * move) {
  if(!board)
    return -1;
  player_t oppositeColor = PLAYER_EMPTY;
  if(color == PLAYER_BLUE)
    color = PLAYER_YELLOW;
  else
    color = PLAYER_BLUE;

  player_t winner;

  int condition = board_has_winner(board, &winner);

  printf("\n");
  player_t ret;
  for(unsigned int row = 0; row < board_get_height(board); row++) {
    for(unsigned int column = 0; column < board_get_width(board); column++) {
      board_get(board, row, column, &ret);
      printf("%u", ret);
    }
    printf("\n");
  }
  printf("\n\n");

  if(condition == 1) {
    if(step == MAX) {
      return 100;
    } else {
      // When step == MINI is implicit
      return -100;
    }
  } else if(condition == -1) {
    return 0;
  } else if(condition == 0) {
    // When our step is MINI, nodeEvaluation will
    // have the lowest value of the possible paths
    // conversly, when our step is MAX it will
    // instead hold the largest value of the paths
    int result;
    int nodeEvaluation = 0;
    for(int i = 0; i < board_get_width(board); i++) {
      if(board_can_play_move(board, color, i) == false)
        continue;
      board_play(board, i, color);
      if(step == MAX) {
        // Here we want to find the smallest of the possible path values -- the MINI
        result = generate(board, MINI, oppositeColor, move);
        if(result == -1)
          return -1;
        if(result >= nodeEvaluation) {
          nodeEvaluation = result;
          *move = i;
        }
      } else {
        // Here we want to find the largest of the possible path values -- the MAX
        result = generate(board, MAX, oppositeColor, move);
        if(result == -1)
          return -1;
        if(result <= nodeEvaluation) {
          nodeEvaluation = result;
          *move = i;
        }
      }
      // Reset the board to how it was before we generate
      // the paths -- now the other paths can be generated
      // without overlap or interplay
      board_unplay(board, i);
    }

    return nodeEvaluation;
  }

  return -1;
}

int findMove(board_t * board, unsigned int * move, player_t color) {
  if(!board || !move)
    return -1;

  if(generate(board, MAX, color,  move) == -1) {
    return -1;
  }

  return 0;
}


int main(int argc, char ** args) {
  board_t * board;
  board_create(&board, 4, 4, 3, NULL);
  board_play(board, 3, PLAYER_YELLOW);
  board_play(board, 2, PLAYER_BLUE);
  board_play(board, 1, PLAYER_YELLOW);
  board_play(board, 3, PLAYER_BLUE);
  board_play(board, 0, PLAYER_YELLOW);
  board_play(board, 3, PLAYER_BLUE);
  board_play(board, 3, PLAYER_YELLOW);
  board_play(board, 0, PLAYER_BLUE);
  board_play(board, 0, PLAYER_YELLOW);
  board_play(board, 0, PLAYER_YELLOW);
  board_play(board, 2, PLAYER_YELLOW);
  unsigned int move;
  board_t * temp = NULL;
  board_duplicate(&temp, board);
  findMove(temp, &move, PLAYER_BLUE);
  printf("perfect move = %i\n", move);
  board_destroy(temp);
  board_destroy(board);
}
