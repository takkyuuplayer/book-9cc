#include "chibicc.h"

//
// Code generator
//

void gen_lval(Node *node)
{
    if (node->kind != ND_LVAR)
    {
        error("LVAR is not an identifier");
    }
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
}

static int jeLabel = 0;

void gen(Node *node)
{
    switch (node->kind)
    {
    case ND_NUM:
        printf("  push %d\n", node->val);
        return;
    case ND_LVAR:
        gen_lval(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    case ND_ASSIGN:
        gen_lval(node->lhs);
        gen(node->rhs);

        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;
    case ND_RETURN:
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
        return;
    case ND_IF:
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        if (!node->els)
        {
            printf("  je .Lend%03d\n", jeLabel);
            gen(node->then);
        }
        else
        {
            printf("  je .Lelse%03d\n", jeLabel);
            gen(node->then);
            printf("  jmp .Lend%03d\n", jeLabel);
            printf(".Lelse%03d:\n", jeLabel);
            gen(node->els);
        }
        printf(".Lend%03d:\n", jeLabel);
        jeLabel++;
        return;
    case ND_WHILE:
        printf(".Lbegin%03d:\n", jeLabel);
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend%03d\n", jeLabel);
        gen(node->then);
        printf("  jmp .Lbegin%03d\n", jeLabel);
        printf(".Lend%03d:\n", jeLabel);
        jeLabel++;
        return;
    default:
        break;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind)
    {
    case ND_ADD:
        printf("  add rax, rdi\n");
        break;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rdi\n");
        break;
    case ND_EQ:
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_NE:
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LT:
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LE:
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        break;
    }

    printf("  push rax\n");
}

int stack_size(LVar *locals)
{
    int i = 0;
    for (LVar *var = locals; var; var = var->next)
    {
        i++;
    }
    return 8 * i;
}

void generate(Program *program)
{
    // Print out the first half of assembly.
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // プロローグ
    // 変数26個分の領域を確保する
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, %d\n", stack_size(program->locals));

    // Traverse the AST to emit assembly.
    for (int i = 0; program->codes[i]; i++)
    {
        gen(program->codes[i]);
        printf("  pop rax\n");
    }

    // エピローグ
    // 最後の式の結果がRAXに残っているのでそれが返り値になる
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
}
