#include <iostream>
#include <fstream>
#include <stdio.h>   
#include <time.h>   
#include <cstring> 
using namespace std;
FILE* createFile(string name){
 
time_t curr_time;
tm * curr_tm;
char date_string[100];

time(&curr_time);
curr_tm = localtime(&curr_time);

strftime(date_string, 50, "%B-%d-%Y", curr_tm);

const char* directory = "../output/";
const char* reportType = name.data();
const char* fileName = strcat(date_string,reportType);
const char* fileType = ".txt";
 
char name_buffer[512];
FILE* f = NULL;
    //Print to character buffer
    sprintf(name_buffer,"%s%s%s",directory,fileName,fileType);
 
    f = fopen(name_buffer,"w");

    cout << "Created " << reportType << endl;
    return f;
}
