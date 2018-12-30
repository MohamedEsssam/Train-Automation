//
// Created by mohamedessam on 12/6/18.
//

#ifndef TRAIN_AUTOMATION_H_
#define TRAIN_AUTOMATION_H_
#include <sys/types.h>
#include <pthread.h>


typedef struct Station{
    int waitingPassengersStatiom;
    int availableSeatsInTrain;
    int trainInStation;
    pthread_mutex_t station_mutex;
    pthread_cond_t train_in_station;
    pthread_cond_t boarding_passingerToTrain;
    pthread_cond_t next_train;
}Station;


typedef struct loadTrain{
    int seatsNum;
    Station * station;
}laodTrain;

//initialize the station object when the system boots
void station_init(struct Station *station);

/*
  1- When a train arrives in the station and has opened its doors
  2- count indicates how many seats are available on the train
  3- The function must notbreturn until the train is satisfactorily loaded (all passengers are in their seats, and
     either the train is full or all waiting passengers have boarded)
*/
void station_load_train(struct Station *station, int count);

/*
  1-the passenger robot will move the passenger on board the train and into a seat
  2-indicate how many enough free seats on the train for this passenger to sit down
  3-This function must not return until a train is in the station
*/
void station_wait_for_train(struct Station *station);

/*
   Once the passenger is seated call this function to let the train know that it's on board.
 */
void station_on_board(struct Station *station);

void * train_thread(void * args);

void * passenger_thread(void * args);

#endif //TRAIN_AUTOMATION_H_
