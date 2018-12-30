//
// Created by mohamedessam on 12/7/18.
//

#include "Train_ Automation.h"
#include <stdio.h>
#include <pthread.h>

int flagUp    = 1 ;
int flagDown  = 0 ;

//initialize Station struct.
void station_init(Station * station)
{
    station->availableSeatsInTrain=0;
    station->waitingPassengersStatiom=0;
    station->trainInStation= flagDown;
    pthread_mutex_init(&station->station_mutex , NULL);
    pthread_cond_init(&station->train_in_station, NULL);
    pthread_cond_init(&station->boarding_passingerToTrain,NULL);
    pthread_cond_init(&station->next_train,NULL);
}

void station_load_train(Station *station, int count){

    pthread_mutex_lock(&station->station_mutex);
    while( station->trainInStation == flagUp)
        pthread_cond_wait(&(station->next_train), &(station->station_mutex));

    //indicate how many train will enter the station
    static int numOfTrains = 0;

    station->availableSeatsInTrain = count;
    station->trainInStation = flagUp;
    numOfTrains++;
    printf("Train %d arrived\n",numOfTrains);
    pthread_cond_broadcast(&(station->train_in_station));
    while( (station->availableSeatsInTrain > 0)  && (station->waitingPassengersStatiom > 0))
        pthread_cond_wait(&(station->boarding_passingerToTrain),&station->station_mutex);
    station->trainInStation = flagDown;
    printf("Train %d leaving station with seats left = %d\n",numOfTrains,station->availableSeatsInTrain);
    printf("Remaining passenger waining in station = %d\n",station->waitingPassengersStatiom);
    pthread_cond_signal(&(station->next_train));
    pthread_mutex_unlock(&station->station_mutex);
}

void station_wait_for_train(Station *station){
    pthread_mutex_lock(&station->station_mutex);
    station->waitingPassengersStatiom++;
    printf("Passenger %d waiting\n",station->waitingPassengersStatiom);
    while( station->trainInStation == flagDown || station->availableSeatsInTrain == 0)
        pthread_cond_wait(&(station->train_in_station), &(station->station_mutex));
    printf("Passenger %d boarded\n",station->waitingPassengersStatiom);
    if(station->waitingPassengersStatiom > 0)
        station->waitingPassengersStatiom--;
}

void station_on_board(Station *station){
    if(station->availableSeatsInTrain >= 0 && station->waitingPassengersStatiom > -1) {
      //  station->waitingPassengersStatiom--;
        station->availableSeatsInTrain--;
    }
    printf("Passenger Seated, Remaining Seats is %d and passenger waiting in station is %d \n",station->availableSeatsInTrain,station->waitingPassengersStatiom);
    pthread_cond_signal(&(station->boarding_passingerToTrain));
    pthread_mutex_unlock(&station->station_mutex);
}


void * train_thread(void * args)
{
   struct loadTrain * load =(struct train_threadArgs*) args;
   station_load_train(load->station, load->seatsNum);
   pthread_exit(NULL);
}

void * passenger_thread(void * args)
{
   Station * station=(struct Station*)args;
   station_wait_for_train(station);
   station_on_board(station);
   pthread_exit(NULL);
}
