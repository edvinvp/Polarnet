// Edvin von Platen

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "gnuplot_i.h"
#include <time.h>

#ifndef MEDIALNEURONS
#define MEDIALNEURONS 25
#endif

#ifndef RATE
#define RATE 0.01
#endif

#ifndef LEARNINGITERATIONS
#define LEARNINGITERATIONS 2500
#endif

#define degreesToRadians(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define PI 3.14159265

double medialin[MEDIALNEURONS];
double medialout[MEDIALNEURONS];
// Weights for the 2 layers
double synone[2][MEDIALNEURONS];
double syntwo[MEDIALNEURONS][2];
// Stores the error-reducing signal propagating backwards from the medial neruons
double sigma[MEDIALNEURONS];
// Stores reconstructed output from the medial neurons
double sigmoid[MEDIALNEURONS];

double output[2];
double error[2];
double target[2];
double input[2];

// x y: stores error data / iteration for graphing
double graphX[LEARNINGITERATIONS];
double graphY[LEARNINGITERATIONS];

void initSyn(void);
void coordinateConvert(void);
void backPropogate(void);

// Implementation of the neural network "Polarnet" described in "The (new) Turing Omnibus"
// Trains a network to convert from polar coordinates on the unit circle to cartesian
// coordinates. 
int main()
{
    srand(time(NULL));
    initSyn();
    for (int i = 0; i < LEARNINGITERATIONS; i++) {
        double theta = (double) (rand() % 360);
        // Get random point on unit circle
        input[0] = 1;
        input[1] = degreesToRadians(theta);
        // clamp input values from -1 to 1.
        input[1] = (input[i] - PI) / PI;
        coordinateConvert();
        backPropogate();
        graphX[i] = (double) i / 1000;
        double err = sqrt(error[0] * error[0] + error[1] * error[1]);
        graphY[i] = err;
    }
    // Send data to gnuplot and display for 20 seconds.
    gnuplot_ctrl *h = gnuplot_init();
    gnuplot_setstyle(h,"points");
    gnuplot_plot_xy(h, graphX, graphY, LEARNINGITERATIONS, "user-defined points");
    printf("\"q\" to quit\n");
    char c;
    while ((c = getc(stdin)) != 'q')
        ;
    gnuplot_close(h);
}

// initialize synone and syntwo to low weights < 0.1
void initSyn(void)
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < MEDIALNEURONS; j++) {
            synone[i][j] = 0.1 * ((double)rand() /(double) RAND_MAX);
            syntwo[j][i] = 0.1 * ((double)rand() /(double) RAND_MAX);
        }
    }
}

// Convert from polar to cartesian by traveling through the net.
// Compares the net output with the target coordinates and records
// the error rate.
void coordinateConvert(void)
{
    for (int i = 0; i < MEDIALNEURONS; i++) {
        medialin[i] = 0;
        for (int j = 0; j < 2; j++) 
            medialin[i] = medialin[i] + synone[j][i] * input[j];      
        medialout[i] = tanh(medialin[i]);
    }

    for (int i = 0; i < 2; i++) {
        output[i] = 0;
        for (int j = 0; j < MEDIALNEURONS; j++)
            output[i] = output[i] + syntwo[j][i] * medialout[j];
        target[i] = (i == 0) ? cos(input[1]) : sin(input[1]);
        error[i] = target[i] - output[i];
    }
}

// Adjust the synaptic weights backwards through the net.
void backPropogate(void)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < MEDIALNEURONS; j++)
            syntwo[j][i] = syntwo[j][i] + RATE * medialout[j] * error[i];
    for (int i = 0; i < MEDIALNEURONS; i++) {
        sigma[i] = 0;
        for (int j = 0; j < 2; j++)
            sigma[i] = sigma[i] + error[i] * syntwo[i][j];
        sigmoid[i] = 1 - (medialin[i] * medialin[i]); // derivate of the ssigmoid function 1-y^2
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < MEDIALNEURONS; j++) {
            double delta = RATE * sigmoid[j] * sigma[j] * input[i];
            synone[i][j] = synone[i][j] + delta;
        }
    }
}
