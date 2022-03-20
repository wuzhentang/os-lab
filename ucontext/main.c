#include <stdlib.h>
#include <stdio.h>

#include <signal.h>
#include <ucontext.h>
#include <unistd.h>

ucontext_t produce_cxt, consumer_cxt;

int current = 0;

void produce() {
    current++;
    printf("product current vaule:%d\n", current);
    setcontext(&consumer_cxt);
 }

void consumer() {
    printf("consumer current vaule:%d\n", current);
    if (current == 10) {
        exit(0);
    }
    setcontext(&produce_cxt);
}


int main(int argc, char* argv[]) {
    char produce_stack[SIGSTKSZ];
    char consumer_stack[SIGSTKSZ];

    getcontext(&produce_cxt);
    produce_cxt.uc_link = NULL;
    produce_cxt.uc_stack.ss_sp = produce_stack;
    produce_cxt.uc_stack.ss_size = sizeof(produce_stack);
    makecontext(&produce_cxt, (void (*)(void))produce, 0);



    getcontext(&consumer_cxt);
    consumer_cxt.uc_link = NULL;
    consumer_cxt.uc_stack.ss_sp = consumer_stack;
    consumer_cxt.uc_stack.ss_size = sizeof(consumer_stack);
    makecontext(&consumer_cxt, (void (*)(void))consumer, 0);

    setcontext(&produce_cxt);

    return 0;
 }
