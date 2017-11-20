
# The Bartender (Problem)

## base code @author David Chiu // 
## adapted code @author Thavy Thach // April 8th, 2017

# Description

This is a simulation of a bar establishment using C's sychronization mechanisms. This bar has a capacity and that capacity is maxed out at 2 (only the bartender and the customer) which means only they may be in the bar at any time. This is how the simulation is broken down. There is a certain amount of time a customer travels to the bar. If there is already customer at the bar, then that person and all those before that customer must wait causing a line outside. The customer inside will order and begin browsing while the drink is made. The bartender will now receive the order and being mixing drinks and then wait for customer to get to the register after browsing the wall for some amount of time. The bartender then waits for the pay. The customer then pays and leaves.

# How to make this work?

[1] cd c/
[3] make 
[4] ./thebar [num of customers]