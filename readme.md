# Nerdy traffic congestion!

Background: Jinan transportation department hopes to introduce new technologies to build an intelligent transportation system integrating traffic management, control, decision-making and service. Under the guidance of Professor Cui Yuquan, I have carried out the modeling and optimization of intersection signal timing.

Data processing: on the basis of outlier processing, traffic parameters such as road carrying capacity and vehicle trip frequency are estimated.

Modeling and Optimization: the objective is to minimize the average delay time of intersection(measured by Webster formula), and the constraint conditions are traffic flow conservation, Lane saturation constraint, Lane constraint, signal duration range, etc. Genetic algorithm (GA) is used to optimize the estimated parameters to get the optimized signal timing.

This repository contains the solution for nonlinear programming by GA encoded by C++ because others are a little bit trivial.

Project results: since 2019, the average delay time of morning and evening peak hours in Jinan has decreased by 10.73% and 10.94%.

![image](https://github.com/Slam1423/Traffic-Congestion-GA/blob/main/crosses.png)
