#include <string.h>
#include <stdbool.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <stdio.h>
#include <assert.h>
#include <debug.h>
#include "../../../include/isa.h"
#include <memory/paddr.h>
enum {
  TK_NOTYPE = 256, TK_EQ,
  /* TODO: Add more token types */
  TK_NUM, TK_OP, TK_NEQ, TK_AND, TK_REG, TK_HEX, DEREF
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"\\$[a-z][0-9]+", TK_REG},
  {"\\$[a-z]+", TK_REG},
  {"0x[0-9]+", TK_HEX},
  {"&&", TK_AND},
  {"!=", TK_NEQ},
  {"\\)", ')'},
  {"\\(", '('},
  {"\\*", '*'},
  {"\\/", '/'},
  {"-", '-'},
  {"\\+", '+'}, 
  {"[0-9]+", TK_NUM},
  {" +", TK_NOTYPE},    // spaces
        // plus
  {"==", TK_EQ},        // equal
};
#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      printf("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;
word_t eval(int p, int q);
int check_parentheses(int p, int q);

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        if (substr_len > 32) {
          printf("Overflow!\n");
          assert(0);
        }
        Token temp;
        memset(temp.str, 0, 32);
        switch (rules[i].token_type) {
          case TK_NOTYPE:
            temp.type = TK_NOTYPE;
            strcpy(temp.str, " ");
            break;
          case TK_NUM:
            temp.type = TK_NUM;
            strncpy(temp.str, e + position - substr_len, substr_len);
            break;
          case TK_EQ:
            temp.type = TK_EQ;
            strncpy(temp.str, e + position - substr_len, substr_len);
            break;
          case TK_AND:
            temp.type = TK_AND;
            strncpy(temp.str, e + position - substr_len, substr_len);
            break;
          case TK_NEQ:
            temp.type = TK_NEQ;
            strncpy(temp.str, e + position - substr_len, substr_len);
            break;
          case TK_HEX:
            temp.type = TK_HEX;
            strncpy(temp.str, e + position - substr_len, substr_len);
            break;
          case TK_REG:
            temp.type = TK_REG;
            strncpy(temp.str, e + position - substr_len, substr_len);
            break;
          case '+':
          case '-':
          case '*':
          case '/':
          case '(':
          case ')':
            temp.type = TK_OP;
            strncpy(temp.str, e + position - substr_len, substr_len);
            break;
        }
        tokens[nr_token ++] = temp;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */

  for (int i = 0; i < nr_token; i ++)
  {
    if (tokens[i].type == TK_REG) {
      bool success = true;
      word_t val = isa_reg_str2val(tokens[i].str + 1, &success);
      assert(success == true);
      sprintf(tokens[i].str, "%u", val);
    }
  }
  for (int i = 0; i < nr_token; i ++) 
  {
    if (tokens[i].str[0] == '*' && (i == 0 || 
      (tokens[i-1].type == TK_OP && tokens[i-1].str[0] != ')') || 
      (tokens[i-1].type == TK_AND || tokens[i-1].type == TK_EQ || tokens[i-1].type == TK_NEQ)
      ) ) 
    {
      if (tokens[i+1].type == TK_REG)
      {
        paddr_t addr = 0;
        sscanf(tokens[i+1].str, "%d", &addr);
        printf("addr: %x\n", addr);
        word_t val = paddr_read(addr, 4);
        tokens[i+1].type = TK_NUM;
        sprintf(tokens[i+1].str, "%u", val); 
      }
      if (tokens[i+1].type == TK_HEX) 
      {
        paddr_t addr = 0;
        sscanf(tokens[i+1].str, "%x", &addr);
        //printf("look for %x\n", addr);
        word_t val = paddr_read(addr, 4);
        printf("look for %x, find %x\n", addr, val);
        tokens[i+1].type = TK_NUM;
        sprintf(tokens[i+1].str, "%u", val); 
      }
      tokens[i].type =  TK_NOTYPE;
    }
  }
  int nr_temp = 0;
  for (int i = 0; i < nr_token; i ++)
  {
    if (tokens[i].type != TK_NOTYPE)
      tokens[nr_temp ++] = tokens[i];
  }
  nr_token = nr_temp;
  word_t val = eval(0, nr_token -1);
  //printf("val: %d\n", val);
  return val;
}

int check_parentheses(int p, int q) {
  int left = 0;
  for (int i = p; i <= q; i ++)
  {
    if (tokens[i].str[0] == '(')
      left ++;
    if (tokens[i].str[0] == ')')
    {
      if (left == 0)
        return -1;
      else left --;
    }
  }
  if (left != 0) 
    return -1;
  if (tokens[p].str[0] == '(' && tokens[q].str[0] == ')')
  {
    if (check_parentheses(p+1, q-1) != -1)
      return 1;
  }
  return 0;
}

word_t eval(int p, int q)
{
  if (p > q) {
    assert(0);
  } 
  else if (p == q) {
    return atoi(tokens[p].str);
  }
  else if (check_parentheses(p, q) == 1) {
    return eval(p + 1, q - 1);
  } 
  else if (check_parentheses(p, q) == -1) {
    assert(0);
  }
  else {
    int main_op_index = p-1;
    char ops[4] = {'+', '-', '*', '/'};
    for (int i = p; i <= q; i ++)
    {
      if (tokens[i].str[0] == ops[0] || tokens[i].str[0] == ops[1] || tokens[i].str[0] == ops[2] || tokens[i].str[0] == ops[3]) 
      {
        int le = 0;
        for (int k = p; k < i; k ++)
        {
          if (tokens[k].str[0] == '(')
            le ++;
          if (tokens[k].str[0] == ')')
            le --;
        }
        if (le)
          continue;
        else if (main_op_index == p-1) {
          main_op_index = i;
        } else if (tokens[main_op_index].str[0] == '*' || tokens[main_op_index].str[0] == '/') {
          main_op_index = i;
        } else if (tokens[i].str[0] == '+' || tokens[i].str[0] == '-') {
          main_op_index = i;
        }
      }
    }
    
    word_t val1 = eval(p, main_op_index - 1);
    word_t val2 = eval(main_op_index + 1, q);
    switch (tokens[main_op_index].str[0])
    {
      case '+': return val1 + val2; break;
      case '-': return val1 - val2; break;
      case '*': return val1 * val2; break;
      case '/': return val1 / val2; break;
      default: assert(0);
    }
  }
}