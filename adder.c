#include <stdio.h>
#include <string.h>

#define MAXSIZE 8192

void tt1(char *name, unsigned (*f)(unsigned))
{
    unsigned name_len = strlen(name);
    char line[1024];
    unsigned i;

    for (i = 0; i < name_len; i++) {
        line[i] = '-';
    }
    line[i] = '\0';

    printf("\n A | %s  \n", name);
    printf("---+-%s- \n", line);

    for (i = 0; i < 2; i++) {
        unsigned a = i&1;

        printf(" %u | %*s%u \n", a, name_len / 2, "", f(a)&1);
    }
}

void tt2(char *name, unsigned (*f)(unsigned, unsigned))
{
    unsigned name_len = strlen(name);
    char line[1024];
    unsigned i;

    for (i = 0; i < name_len; i++) {
        line[i] = '-';
    }
    line[i] = '\0';

    printf("\n A | B | %s  \n", name);
    printf("---+---+-%s- \n", line);

    for (i = 0; i < 4; i++) {
        unsigned a = (i>>1)&1;
        unsigned b = i&1;

        printf(" %u | %u | %*s%u \n", a, b, name_len / 2, "", f(a,b)&1);
    }
}

void tt3(char *name, unsigned (*f)(unsigned, unsigned, unsigned))
{
    unsigned name_len = strlen(name);
    char line[1024];
    unsigned i;

    for (i = 0; i < name_len; i++) {
        line[i] = '-';
    }
    line[i] = '\0';

    printf("\n A | B | C | %s  \n", name);
    printf("---+---+---+-%s- \n", line);

    for (i = 0; i < 8; i++) {
        unsigned a = (i>>2)&1;
        unsigned b = (i>>1)&1;
        unsigned c = i&1;

        printf(" %u | %u | %u | %*s%u \n", a, b, c, name_len / 2, "", f(a,b,c)&1);
    }
}

/////////////////////////////

unsigned NAND(unsigned a, unsigned b)
{
    return ~(a & b);
}

unsigned NOT(unsigned a)
{
    return NAND(a, a);
}

unsigned AND(unsigned a, unsigned b)
{
    return NOT(NAND(a, b));
}

unsigned OR(unsigned a, unsigned b)
{
    return NAND(NOT(a), NOT(b));
}

unsigned NOR(unsigned a, unsigned b)
{
    return NOT(OR(a, b));
}

unsigned XOR(unsigned a, unsigned b)
{
    return OR(AND(NOT(a), b), AND(a, NOT(b)));
}

unsigned half_adder_carry(unsigned a, unsigned b)
{
    return AND(a, b);
}

unsigned half_adder_sum(unsigned a, unsigned b)
{
    return XOR(a, b);
}

#if 0
A  B  Ci  |  Co  S
0  0  0       0  0
0  0  1       0  1
0  1  0       0  1
0  1  1       1  0
1  0  0       0  1
1  0  1       1  0
1  1  0       1  0
1  1  1       1  1

NOT A AND B AND C  OR
A AND NOT B AND C  OR
A AND B AND NOT C  OR
A AND B AND C

Co = A AND (B OR Ci) OR NOT A AND (B AND Ci)
S = A AND (B XOR Ci)   OR   NOT A AND NOT(B XOR Ci)
#endif

unsigned full_adder_carry(unsigned a, unsigned b, unsigned c)
{
    return OR(AND(OR(b, c), a), AND(AND(b, c), NOT(a)));
}

unsigned full_adder_sum(unsigned a, unsigned b, unsigned c)
{
    return XOR(XOR(a, b), c);
}


unsigned adder(unsigned a1, unsigned a0, unsigned b1, unsigned b0)
{
    unsigned s0 = full_adder_sum(a0, b0, 0);
    unsigned c0 = full_adder_carry(a0, b0, 0);

    unsigned s1 = full_adder_sum(a1, b1, c0);
    //unsigned c1 = full_adder_carry(a1, b1, c0); overflow

    return (s1<<1) | s0;
}

unsigned adder3(unsigned a2, unsigned a1, unsigned a0, unsigned b2, unsigned b1, unsigned b0)
{
    unsigned s0 = full_adder_sum(a0, b0, 0);
    unsigned c0 = full_adder_carry(a0, b0, 0);

    unsigned s1 = full_adder_sum(a1, b1, c0);
    unsigned c1 = full_adder_carry(a1, b1, c0);

    unsigned s2 = full_adder_sum(a2, b2, c1);
    //unsigned c2 = full_adder_carry(a2, b2, c1);

    return (s2<<2) | (s1<<1) | s0;
}

int main(void)
{
    tt2("A NAND B", NAND);
    tt1("NOT A", NOT);
    tt2("A AND B", AND);
    tt2("A OR B", OR);
    tt2("A NOR B", NOR);
    tt2("A XOR B", XOR);

#if 0
    unsigned carry = half_adder_carry(1, 1);
    unsigned sum = half_adder_sum(1, 1);

    printf("%d %d\n", carry, sum);
#endif

    tt3("Cout", full_adder_carry);
    tt3("S", full_adder_sum);

    printf("0 + 0: %d\n", adder(0,0, 0,0));
    printf("0 + 1: %d\n", adder(0,0, 0,1));
    printf("1 + 1: %d\n", adder(0,1, 0,1));
    printf("1 + 2: %d\n", adder(0,1, 1,0));
    printf("0 + 2: %d\n", adder(0,0, 1,0));
    printf("2 + 2: %d\n", adder(1,0, 1,0)); // 0b100
    printf("2 + 3: %d\n", adder(1,0, 1,1)); // 0b101
    printf("3 + 3: %d\n", adder(1,1, 1,1)); // 0b110

    printf("1 + 5: %d\n", adder3(0,0,1, 1,0,1));
    printf("2 + 5: %d\n", adder3(0,1,0, 1,0,1));
    printf("4 + 7: %d\n", adder3(1,0,0, 1,1,1)); // 0b1011
}