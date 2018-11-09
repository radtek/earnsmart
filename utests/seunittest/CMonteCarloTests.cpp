/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CMonteCarloTests.cpp
 * Author: santony
 * 
 * Created on August 7, 2016, 9:27 PM
 */

#include "CMonteCarloTests.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>

CMonteCarloTests::CMonteCarloTests() 
{
}

CMonteCarloTests::CMonteCarloTests(const CMonteCarloTests& orig) {
}

CMonteCarloTests::~CMonteCarloTests() {
}

/* computation of integral
 I = integral of [ 1/(2pi^3) * 1/(1-cos(x)cos(y)cos(z)) (dx, dy, dz)
 * 
 * over -pi, -pi, -pi to +pi, +pi, +pi (-180 to +180)
 * 
 * The exact answer is Gamma (1/4)^4 / ( 4 pi^3)
 * 
 */

const double exact = 1.3932039296856768591842462603255;

double CMonteCarloTests::g(double* k, size_t dim, void* params) 
{
    (void)(dim); // avoid using unused parameters
    (void)(params);
    double A = 1.0 / (M_PI * M_PI * M_PI);
    return A / (1.0 - cos(k[0]) * cos(k[1]) * cos(k[2]) );
}

void CMonteCarloTests::displayResults(char* title, double result, double error) 
{
    printf("%s==============================\n", title);
    printf("result = % .6f\n", result);
    printf("sigma = % .6f\n", error);
    printf("exact = % .6f = %.2g sigma\n", result - exact, fabs(result-exact)/error);
}

void CMonteCarloTests::setUp() 
{

}

void CMonteCarloTests::tearDown() 
{

}


void CMonteCarloTests::testPlain() 
{

}
