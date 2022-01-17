#include <iostream> 
#include <string>
#include <iomanip> 
#include <cctype>
#include <cmath>
#include <chrono>
#include <ctime>
#include <fstream>

using namespace std; 

/*
Alex Urbanski
CIS-2541 Final Project
12/07/2021

This program is a productivity survey designed specifically for me
Program saves all data collected to a struct, then writes the data to a text file so I can save a log of what I did every day 
Specifics:
- program asks about workout and various workout statistics that are relative to the workout
- program asks about time spent studying
- program asks about what I listened to while driving
- program asks about sleep
- data is implemented into exponential growth models to generate a productivity score
- mathematical formulas are designed specifically to my goals 
- feedback on activities throughout the day along with a motivational video is provided at end of survey
*/


struct vids{
    // struct to hold motivational video links
    string goggins_workout = "https://youtu.be/jUqLtrPDsxQ";
    string goggins_snooze = "https://www.youtube.com/watch?v=tmr-tDZzhEw";
    string goggins_run = "https://youtu.be/NrRvP5EK990";
    };

struct stats{
    char workout_y_or_n; // y for workout, n for n workout
    string activity; // string will hold which activity I did for that workout
    // workout specifics
    double speed ;
    double distance ;
    double workout_val; // final workout value
    char c_or_i ; // complete or incomplete lift
    char u_or_l ; // upper body or lower body lift 
    double study_hours; // quantity of hours spent studying
    char wake_up[5]; // holds the time woken up at in a c-string
    bool snooze; // boolean that holds whether I snoozed my alarm or not
    double score; // final score
    int listen_item; // what was listened to while driving
    string listened_to; // string of what I listened to while driving
    double hours_driven; //  quantity of hours spent driving
    int sleep_val ; // final sleep value
    int commute_val ; //  final commute value
    double study_val; //  final study value
    int snoozex; // counts how many times alarm was snoozed
    vids v; // motivational video links
    
};

double clamp(double num,int bound1,int bound2){
    // ensure the returned number is within a certain range
    if (num < bound1){
        return bound1;}
    else if (num > bound2){
        return bound2;}
    else{
        return num;}
}

void workout_question(stats &p){
    int min, score, workout_length, activity;

    // validate user input
    do{
        cout << "\n\nDid you workout today (y or n)?  ";
        cin >> p.workout_y_or_n;
        tolower(p.workout_y_or_n);
    } while (!(p.workout_y_or_n == 'n' || p.workout_y_or_n == 'y'));
    
    
    if (p.workout_y_or_n == 'n'){
        p.workout_val = 0;}
    else
        {
        cout << "\nRun.....1\nBike.....2\nWeights.....3 \nWhat was the activity for your workout?  ";
        cin >> activity;
        switch (activity)
        {
        case 1:{
            p.activity = "Run";
            // run - distance should be largest weight (tyring to build endurance)
            cout << "\nSpeed (in mph):  ";
            cin >> p.speed;
            cout << "\nDistance (in miles):  ";
            cin >> p.distance; 
            p.workout_val = clamp((pow((log10(p.distance)*10.0),2.95))/(7.0/p.speed),0,150);
            break;}

        case 2:{
            p.activity = "Bike";
            // bike ride
            cout << "\nAverage Pace (in mph):  ";
            cin >> p.speed;
            cout << "\nDistance (in miles):  ";
            cin >> p.distance; 
            p.workout_val = clamp(pow(p.distance,3)/(pow(12/p.speed,2)),0,100);
            break;}
        
        case 3:{
            p.activity = "Weights";
            char upper_or_lower, c_or_i;
            int num1, num2;
            // weight lifting 
            cout << "\nWorkout length (complete-c or incomplete-i):  ";
            cin >> p.c_or_i;
            tolower(p.c_or_i);
            cout << "\nUpper body or lower body (u or l)?  ";
            cin >> p.u_or_l;
            tolower(p.u_or_l);
            if (p.c_or_i == 'c'){
                num1 = 40;}
            else  {
                num1 = 15;}
            if (p.u_or_l == 'u'){
                num2 = 40;}
            else{
                num2 = 15;}
            p.workout_val = num1+num2;
            break;
        }
        
        default:
            break;
        }

        }
}
void study_question(stats &p){
    double val;

    // validate user input
    do{
    cout << "\n\nHow many hours did you study for today?  ";
    cin >> p.study_hours;
    } while (p.study_hours < 0);

    p.study_val = clamp(pow(log10(p.study_hours)*12,2.04),0,150); 
    
}

void sleep_question(stats &p){
    char c;
    int wake_hour, sub; 
    
    do{
    cout << "\n\nWhat time did you wake up today (format 8:00)?  ";
    cin.getline(p.wake_up,5);
    } while (!(p.wake_up[1]==':' || p.wake_up[2]==':'));
    
    if (p.wake_up[1] == ':'){
        wake_hour = ((int)(*(p.wake_up)))-48;
        } // subtract 48 to get real integer value due to ascii conversion
    else
        {
        wake_hour = 9; // can be any number above 8 (so it will eval to false in later if statement)
        }
    // validate input
    do {
    cout << "\nDid you snooze your alarm today (y or n)?  ";
    cin >> c;
    tolower(c);
    } while (!(c== 'y' || c== 'n'));
    // if I snoozed, then ...
    if (c == 'y'){
        cout << "\nHow many times?  "; // ask how many times I snoozed
        cin >> p.snoozex;
        sub = 30 * p.snoozex; // subtract 30 * how every many times I snoozed
        p.snooze = true; // set snooze to true 
    }
    else
        { // subtract nothing
        sub = 0;
        }

    if (wake_hour <= 8){ //if I woke up at or before 8am +100
        p.sleep_val = 100;}
    else{
        p.sleep_val = 0; } // if after 8am, 0 points
    p.sleep_val = p.sleep_val - sub; 
    // score range = -infinity --> 100
}
void commute_question(stats &p){
    
    // I commute for about 1.5-2 hours every day, so it would be unproductive to not
    // listen to an audiobook or podcast
    cout << "\n\nTime spent driving (in hours):  ";
    cin >> p.hours_driven;
    do {
    cout << "\nWhat did you listen to while driving today? \nPodcast.....1\nAudiobook.....2\nMusic.....3\nEnter:  ";
    cin >> p.listen_item;
    } while (!(p.listen_item == 1 || p.listen_item ==2 || p.listen_item == 3));

    switch(p.listen_item){
        case 1:{
            p.listened_to = "podcast";
            p.commute_val = 50; // podcast +50
            break;}
        case 2:{
            p.listened_to = "audiobook";
            p.commute_val = 75; // audiobook +75
            break;}
        case 3:{
            p.listened_to = "music";
            p.commute_val = -30; // music -30
            break;}
        default:{
            break;}
    }
    // score range = -30 --> 75
}

void calc_final_score(stats &p){
    // sets score var in struct to the final score (all scores added together)
    p.score = p.study_val + p.sleep_val + p.commute_val + p.workout_val;
}

void write_to_file(stats &p){
    ofstream outfile; 
    outfile.open("report.txt");
    // write to file sleep info
    outfile << "Woke up at ";
    for (int i=0; i < 5; i++){
        outfile << p.wake_up[i];
    }
    outfile << " AM" << endl;
    if (p.snooze){
        outfile << "Total snoozes: " << p.snoozex << endl;
    }
    

    // write report about workout
    if (p.workout_y_or_n == 'n'){ // if no workout
        outfile << "Did not workout today" << endl;
    }
    else 
        {
            outfile << "\nWorkout activity: " << p.activity << endl;
            if (p.activity != "Weights"){
                outfile << "Distance: " << p.distance << " miles" << endl;
                outfile << "Average speed: " << p.speed << " MPH" << endl;
            }
            else{
                if (p.c_or_i == 'c'){
                    outfile << "Complete lift" << endl;}
                else {
                    outfile << "Incomplete lift" << endl;}
                if (p.u_or_l == 'u'){
                    outfile << "Upper body" << endl;}
                else{
                    outfile << "Lower body" << endl;}
            }
        }
    
    // write report about driving 
    outfile << "\nDrove for " << p.hours_driven << " hours and listened to a/an " << p.listened_to << endl;

    // write report about studying
    outfile << "\nStudied for " << p.study_hours << " hours today" << endl;

    // write scores to file
    outfile << "\n\nScores:" << endl;
    outfile << "Workout score: " << p.workout_val << endl;
    outfile << "Sleep score: " << p.sleep_val << endl;
    outfile << "Study score: " << p.study_val << endl;
    outfile << "Commuting score: " << p.commute_val << endl;
    outfile << "\nTOTAL: " << p.score << endl;
    
}


void offer_advice(stats &p){

    // if I didn't workout, show me a video of David Goggins yelling at me
    if (p.workout_y_or_n == 'n'){
        cout << "GO WORKOUT RIGHT NOW!!!" << endl;
        cout << p.v.goggins_workout << endl;
    }

    // if I hit the snooze button in the morning, show video of people yelling to not hit snooze button
    if (p.snooze){
        cout << "YOUR WAKING UP FAILING!! STOP HITTING THE SNOOZE BUTTON!!" << endl;
        cout << p.v.goggins_snooze << endl;
    }

    // if I ran less than 2 miles, recommend run motivation video
    if (p.distance < 2 && p.activity == "Run"){
        cout << "RUN LONGER! TIRED IS ONLY A MINDSET!" << endl;
        cout << p.v.goggins_run << endl;
    }

}

int main(){
    stats p; // initialize struct that holds all data 

    // ask questions
    sleep_question(p); 
    workout_question(p);
    commute_question(p);
    study_question(p);

    // display and calculate final score
    cout << setw(2) << setprecision(3) << endl;
    calc_final_score(p);
    cout << "FINAL SCORE: " << p.score << " / 475" << endl;

    // write report
    write_to_file(p); 

    // offer advice
    offer_advice(p);


    // command: g++ -std=c++11 productivity_project.cpp

}
