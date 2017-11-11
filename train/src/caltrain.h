#include <pthread.h>
#include <stdio.h>

struct station {
	int passengers;
	int count;
	int freeseat;
	int onboard;
	pthread_mutex_t pass_lock;
	pthread_mutex_t train_lock;
	pthread_cond_t cond_train;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
