#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
  game_state_t* state = (game_state_t*) malloc(sizeof(game_state_t));
  state->x_size = 14;
  state->y_size = 10;
  state->board = (char**) malloc(10 * sizeof(char*));
  for (int y = 0; y < 10; y++){
    char* s = (char*) malloc(sizeof(char) * 16);
    s[15] = '\0';
    s[14] = '\n';
    if (y == 0 || y == 9){
      for (int i = 0; i <= 13; i++)
      {
        s[i] = '#';
      }
    }
    else{
      for (int i = 0; i <= 13; i++)
      {
        if (i == 0 || i == 13){
          s[i] = '#';
        }
        else if (y == 2 && i == 9)
        {
          s[i] = '*';
        }
        else if (y == 4 && i == 4)
        {
          s[i] = 'd';
        }
        else if (y == 4 && i == 5){
          s[i] = '>';
        }
        else{
          s[i] = ' ';
        }
      }
    }

    state->board[y] = s;
  }
  state->num_snakes = 1;

  snake_t* snake = (snake_t*) malloc(sizeof(snake_t));
  snake[0].head_x = 5;
  snake[0].head_y = 4;
  snake[0].tail_x = 4;
  snake[0].tail_y = 4;
  snake[0].live = true;

  state->snakes = snake;

  return state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  free(state->snakes);
  for(int i = 0; i < state->y_size; i++){
    free(state->board[i]);
  }
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  for (int i = 0; i < state->y_size; i++)
  {
    fprintf(fp, "%s", state->board[i]);
  }
  
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
  switch (c)
  {
  case 'w':
  case 's':
  case 'a':
  case 'd':
    return true;
  default:
    return false;
  }
}

static bool is_snake(char c) {
  // TODO: Implement this function.
  switch (c)
  {
  case 'w':
  case 's':
  case 'a':
  case 'd':
  case '>':
  case '<':
  case '^':
  case 'v':
    return true;
  default:
    return false;
  }
}

static char body_to_tail(char c) {
  // TODO: Implement this function.
  switch (c)
  {
  case '>':
    return 'd';
  case '<':
    return 'a';
  case '^':
    return 'w';
  case 'v':
    return 's';
  default:
    return -1;
  }
}

static int incr_x(char c) {
  // TODO: Implement this function.
  if (c == '>' || c == 'd'){
    return 1;
  }
  else if (c == '<' || c == 'a')
  {
    return -1;
  }
  
  return 0;
}

static int incr_y(char c) {
  // TODO: Implement this function.
  if (c == 'v' || c == 's'){
    return 1;
  }
  else if (c == '^' || c == 'w')
  {
    return -1;
  }
  
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  // TODO: Implement this function.
  snake_t sanke = state->snakes[snum];
  char head = get_board_at(state, sanke.head_x, sanke.head_y);
  return get_board_at(state, sanke.head_x + incr_x(head), sanke.head_y + incr_y(head));
}

/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  snake_t* snake = &state->snakes[snum];
  char head = get_board_at(state, snake->head_x, snake->head_y);
  snake->head_x = snake->head_x + incr_x(head);
  snake->head_y = snake->head_y + incr_y(head);
  set_board_at(state, snake->head_x, snake->head_y, head);
  
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  // TODO: Implement this function.
  snake_t* snake = &state->snakes[snum];
  char tail = get_board_at(state, snake->tail_x, snake->tail_y);
  set_board_at(state, snake->tail_x, snake->tail_y, ' ');
  snake->tail_x = snake->tail_x + incr_x(tail);
  snake->tail_y = snake->tail_y + incr_y(tail);
  tail = get_board_at(state, snake->tail_x, snake->tail_y);
  set_board_at(state, snake->tail_x, snake->tail_y, body_to_tail(tail));
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  char next;
  for(int i = 0; i < state->num_snakes; i++){
    next = next_square(state, i);
    if (is_snake(next) || next == '#')
    {
      set_board_at(state, state->snakes[i].head_x, state->snakes[i].head_y, 'x');
      state->snakes[i].live = false;
    }
    else if (next == '*')
    {
      update_head(state, i);
      add_food(state);
    }
    else{
      update_head(state, i);
      update_tail(state, i);
    }
    
    
  }
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  FILE* ptr;
  game_state_t* state = (game_state_t*) malloc(sizeof(game_state_t));
  char** board = NULL;
  int y = 0, x = 0;
  char ch;
  char* row = NULL;
  bool find_x = false;
  ptr = fopen(filename, "r");

  if (NULL == ptr) {
    printf("file can't be opened \n");
  }

  while (!feof(ptr)) {
    ch = fgetc(ptr);
    if (!find_x)
    {
      if (row == NULL){
        row = (char*) malloc(sizeof(char) * 2);
      }
      else{
        row = (char*) realloc(row, sizeof(char) * (strlen(row) + 2));
      }
    }
    row[x++] = ch;
    if (ch == '\n'){
      if (board == NULL)
      {
        board = (char**) malloc(sizeof(char*) * ++y);
      }
      else{
        board = (char**) realloc(board, sizeof(char*) * ++y);
      }
      char* s = malloc(sizeof(char) * (strlen(row) + 1));
      state->x_size = strlen(row) - 1;
      strcpy(s, row);
      board[y - 1] = s;
      strcat(row, "");
      find_x = true;
      x = 0;
    }
  }
  fclose(ptr);
  state->board = board;
  state->y_size = y;
  if(row != NULL){
    free(row);
  }
  return state;
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  unsigned int tail_x = state->snakes[snum].tail_x;
  unsigned int tail_y = state->snakes[snum].tail_y;
  char body = get_board_at(state, tail_x, tail_y);
  char previous_body;
  
  do
  { 
    previous_body = body;
    tail_x += incr_x(body);
    tail_y += incr_y(body);
    body = get_board_at(state, tail_x, tail_y);
  } while (is_snake(body));

  state->snakes[snum].head_x = tail_x - incr_x(previous_body);
  state->snakes[snum].head_y = tail_y - incr_y(previous_body);
  
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  int num_snakes = 0;
  snake_t* snakes = NULL;
  for (int i = 0; i < state->y_size; i++)
  {
    for (int j = 0; j < state->x_size; j++)
    {
      if (is_tail(state->board[i][j]))
      {
        num_snakes++;
        if (snakes == NULL){
          snakes = (snake_t*) malloc(sizeof(snake_t));
        }
        else{
          snakes = (snake_t*) realloc(snakes, sizeof(snake_t) * num_snakes);
        }
        state->snakes = snakes;
        snake_t* snake = (snake_t*) malloc(sizeof(snake_t));
        snake->live = true;
        snake->tail_x = j;
        snake->tail_y = i;
        state->snakes[num_snakes - 1] = *snake;
        find_head(state, num_snakes - 1);
      }
      
    }
    
  }
  state->num_snakes = num_snakes;
  return state;
}
