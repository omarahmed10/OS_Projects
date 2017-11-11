#include "caltrain.h"

void station_init(struct station *station) {
	station->passengers = 0;
	station->count = 0;
	station->onboard = 0;
	station->freeseat = 0;
	pthread_mutex_init(&station->pass_lock, NULL);
	pthread_mutex_init(&station->train_lock, NULL);
	pthread_cond_init(&station->cond_train, NULL);
}

/**
 * When a train arrives in the station and has opened its doors.
 * The function must not return until the train is satisfactorily loaded
 * (all passengers are in their seats, and either the train is full or all waiting passengers have boarded)
 */
void station_load_train(struct station *station, int count) {
	station->count = count;
	station->freeseat = count;
	station->onboard = 0;
	if (count) {
		pthread_cond_broadcast(&station->cond_train);
		pthread_mutex_lock(&station->train_lock);
	}
}

/**
 * When a passenger robot arrives in a station.
 */
void station_wait_for_train(struct station *station) {
	pthread_mutex_lock(&station->pass_lock);
	station->passengers++;
	do {
		pthread_cond_wait(&station->cond_train,
				&station->pass_lock);
	} while (!station->freeseat); //if the train is full(or no train in station) let the passenger locked in the cond_train.
	station->freeseat--;
	pthread_mutex_unlock(&station->pass_lock);
}

/**
 * Once the passenger is seated.
 * to let the train know that it’s on board.
 */
void station_on_board(struct station *station) {
	pthread_mutex_lock(&station->pass_lock);
	station->onboard++;
	station->passengers--;
	if (station->onboard == station->count
			|| !station->passengers) { //check whether all passengers are on board
		// or the train is full
		pthread_mutex_unlock(&station->train_lock);
	}
	pthread_mutex_unlock(&station->pass_lock);
}
