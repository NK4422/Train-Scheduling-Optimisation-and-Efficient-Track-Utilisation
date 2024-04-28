#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#define NUM_STATIONS 5
#define NUM_TRACKS 6
//representing a node (train station)
struct station {
    int id;//station ID
    bool MajorJunction;//True if a major junction, False if a regular railway station
};

//representing an edge (train track)
struct track {
    int source;//source station ID
    int destination;//destination station ID
    bool electrified;//true if track is electrified, false if not
    int maintenance;//maintenance scale (1-100)
    float length;//length of the track
    float weight;//weight of the track
    bool visited;//is the track is visited
};

//creating a new railway station
struct station* createStation(int id, bool MajorJunction) {
    struct station* newStation = (struct station*)malloc(sizeof(struct station));
    newStation->id = id;
    newStation->MajorJunction = MajorJunction;
    return newStation;
}

//creating a new train track
struct track* createTrack(int source, int destination, bool electrified, int maintenance, float length) {
    struct track* newTrack = (struct track*)malloc(sizeof(struct track));
    newTrack->source = source;
    newTrack->destination = destination;
    newTrack->electrified = electrified;
    newTrack->maintenance = maintenance;
    newTrack->length = length;
    return newTrack;
}
//calculating the weight of a track
void calculateWeight(struct track* t, struct station** stations) {
    t->weight = 0.3 * t->length + 0.2 / t->maintenance;
    t->weight += 0.1 * (1 - stations[t->source]->MajorJunction) + 0.1 * (1 - stations[t->destination]->MajorJunction);
    t->weight += 0.3 * (1 - t->electrified);
}
//finding the minimum weight vertex not yet visited
int minWeightVertex(struct track* tracks) {
    int minIdx = -1;
    float minWeight = INT_MAX;
    for (int i = 0; i < NUM_TRACKS; ++i) {
        if (!tracks[i].visited && tracks[i].weight < minWeight) {
            minIdx = i;
            minWeight = tracks[i].weight;
        }
    }
    return minIdx;
}

//Dijkstra's algorithm to find the shortest path
void dijkstra(struct track* tracks[], int src) {
    //setting initial weights to infinity and mark all tracks as not visited
    for (int i = 0; i < NUM_TRACKS; ++i) {
        tracks[i]->weight = INT_MAX;
        tracks[i]->visited = false;
    }

    //setting weight of source track to 0
    tracks[src]->weight = 0;

    for (int count = 0; count < NUM_TRACKS - 1; ++count) {
        //finding the vertex with the minimum weight
        int u = minWeightVertex(tracks);
        if (u == -1) return;//no reachable vertices left

        //marking the selected vertex as visited
        tracks[u]->visited = true;

        //updating weights of adjacent vertices
        for (int v = 0; v < NUM_TRACKS; ++v) {
            if (!tracks[v]->visited && tracks[u]->destination == tracks[v]->source && tracks[u]->weight != INT_MAX
                && tracks[u]->weight + tracks[v]->weight < tracks[v]->weight) {
                tracks[v]->weight = tracks[u]->weight + tracks[v]->weight;
            }
        }
    }
}
int main() {
    //creating railway stations
    struct station* stations[5];
    stations[0] = createStation(0, false);
    stations[1] = createStation(1, true);
    stations[2] = createStation(2, false);
    stations[3] = createStation(3, true);
    stations[4] = createStation(4, false);

    //creating train tracks
    struct track* tracks[6];
    tracks[0] = createTrack(0, 1, true, 80, 10.5);
    tracks[1] = createTrack(0, 2, false, 60, 7.8);
    tracks[2] = createTrack(1, 3, true, 90, 15.2);
    tracks[3] = createTrack(2, 3, false, 70, 12.4);
    tracks[4] = createTrack(3, 4, true, 85, 9.6);
    tracks[5] = createTrack(1, 4, false, 75, 11.3);
    for (int i = 0; i < 6; ++i) {
        calculateWeight(tracks[i], stations);
    }
    int src, dest;
    //statically type in the source station's ID
    src = 0;
    //statically type in the destination station's ID
    dest = 4;
    if(src<dest){
        //running Dijkstra's algorithm
        dijkstra(tracks, src);

        //displaying shortest path
        printf("Safest and efficient path from station %d to station %d:\n", src, dest);
    
        //initialising variables to store the current station and total distance
        int currentStation = dest;
        float totalDistance = 0;

        //traversing the shortest path backwards from the destination to the source
        while (currentStation != src) {
            //finding the track that leads to the current station
            for (int i = 0; i < NUM_TRACKS; ++i) {
                if (tracks[i]->destination == currentStation) {
                    //outputing the current station and the distance to the next station
                    printf("Station %d <- ", currentStation);
                    //updating the total distance
                    totalDistance += tracks[i]->length;
                    //updating the current station to the source of the track
                    currentStation = tracks[i]->source;
                    break;
                }
            }
        }
        printf("Station %d", currentStation);
        printf("\nTotal distance: %.2f km\n", totalDistance);
    }
    else{
        //displaying shortest path
        printf("Safest and efficient path from station %d to station %d:\n", src, dest);
        int temp = src;
        src = dest;
        dest = temp;
        //running Dijkstra's algorithm
        dijkstra(tracks, src);
    
        //initialising variables to store the current station and total distance
        int currentStation = dest;
        float totalDistance = 0;

        //traversing the shortest path backwards from the destination to the source
    
        while (currentStation != src) {
        //finding the track that leads to the current station
            for (int i = 0; i < NUM_TRACKS; ++i) {
                if (tracks[i]->destination == currentStation) {
                    //outputing the current station and the distance to the next station
                    printf("Station %d -> ", currentStation);
                    //updating the total distance
                    totalDistance += tracks[i]->length;
                    //updating the current station to the source of the track
                    currentStation = tracks[i]->source;
                    break;
                }
            }
        }
        printf("Station %d", src);
        printf("\nTotal distance: %.2f km\n", totalDistance);
    }
    return 0;
}
