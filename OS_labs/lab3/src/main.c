#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int a = 0;
int b = 0;
pthread_mutex_t init_m;
pthread_mutex_t mutex;
pthread_cond_t cv;

typedef struct args {
    int p1;
    int p2;
    int rolls;
    long seed;
} args;

void *dice_roll(void *arg) {
    printf("thread start\n");
    args Args = *((args*)arg);
    int roll1, roll2, roll3, roll4;
    int sum1 = Args.p1, sum2 = Args.p2;
    srand(Args.seed);
    pthread_cond_signal(&cv);
    for (int i = 0; i < Args.rolls; ++i) {
        roll1 = (rand() % 6) + 1;
        roll2 = (rand() % 6) + 1;
        roll3 = (rand() % 6) + 1;
        roll4 = (rand() % 6) + 1;
        sum1 += roll1 + roll2;
        sum2 += roll3 + roll4;
    }
    if (sum1 > sum2) {
        pthread_mutex_lock(&mutex);
        a += 1;
        pthread_mutex_unlock(&mutex);
    } else if (sum1 == sum2) {
        pthread_mutex_lock(&mutex);
        b += 1;
        pthread_mutex_unlock(&mutex);
    }
    printf("thread end\n");
    return NULL;
}

int main() {
    int p1, p2, rounds, cur_rnd, tests;
    printf("Enter summary points of player 1 and 2, how many rounds will be in the game and what round it is now:\n");
    scanf("%d %d %d %d", &p1, &p2, &rounds, &cur_rnd);
    printf("Oh, yes, number of tests please: ");
    scanf("%d", &tests);
    args Args;
    Args.p1 = p1;
    Args.p2 = p2;
    Args.rolls = rounds - cur_rnd + 1;
    pthread_t threads[tests];
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&init_m, NULL);
    pthread_cond_init(&cv, NULL);
    for (int i = 0; i < tests; ++i) {
        pthread_mutex_lock(&init_m);
        Args.seed = (time(0) + i);
        pthread_create(&threads[i], NULL, &dice_roll, &Args);
        printf("start waiting for thread signal\n");
        pthread_cond_wait(&cv, &init_m);
        printf("stop waiting\n");
        pthread_mutex_unlock(&init_m);
    }
    for (int i = 0; i < tests; ++i) {
        pthread_join(threads[i], NULL);
    }
    double p1_winrate = (double)a / tests;
    double draw = (double)b / tests;
    double p2_winrate = 1 - p1_winrate - draw;
    printf("Probabilities:\n\tfirst person wins - %.3f, second - %.3f\n\tDraw - %.3f\n Based on %d tests.\n", p1_winrate, p2_winrate, draw, tests);
    return 0;
}