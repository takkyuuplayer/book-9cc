#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char *fmt, ...);

// Tokenizer
typedef enum
{
    TK_RESERVED, // Keywords or punctuators
    TK_IDENT,    // Identifier
    TK_NUM,      // Integer literals
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_FOR,
    TK_EOF, // End-of-file markers
} TokenKind;

// Token type
typedef struct Token Token;
struct Token
{
    TokenKind kind; // Token kind
    Token *next;    // Next token
    int val;        // If kind is TK_NUM, its value
    char *str;      // Token string
    int len;        // Token length
};
Token *tokenize(char *c);

// Parser
typedef enum
{
    ND_ADD,    // +
    ND_SUB,    // -
    ND_MUL,    // *
    ND_DIV,    // /
    ND_EQ,     // ==
    ND_NE,     // !=
    ND_LT,     // <
    ND_LE,     // <=
    ND_NUM,    // Integer
    ND_ASSIGN, // =
    ND_LVAR,   // ローカル変数
    ND_RETURN,
    ND_IF,
    ND_WHILE,
    ND_FOR,
    ND_BLOCK,
} NodeKind;

// AST node type
typedef struct Node Node;
struct Node
{
    NodeKind kind; // Node kind
    Node *lhs;     // Left-hand side
    Node *rhs;     // Right-hand side

    Node *cond;
    Node *then;
    Node *els;

    Node *init;
    Node *inc;

    // Block
    Node *next;

    int val;    // Used if kind == ND_NUM
    int offset; // Used if kind == ND_LVAR
};

typedef struct LVar LVar;

// ローカル変数の型
struct LVar
{
    LVar *next; // 次の変数かNULL
    char *name; // 変数の名前
    int len;    // 名前の長さ
    int offset; // RBPからのオフセット
};

typedef struct Program Program;
struct Program
{
    Node **codes;
    LVar *locals;
};

Program *parse(Token *t);

// Generator
void generate(Program *program);