# CPRE-Optimized-Space

## Inspiration
When we saw the CBRE challenge for TAMUHack 2023, our team was excited to take it on. At first, the problem looked simple to implement, but we quickly realized it was more complex. Although our team faced some difficulties along the way, we found a general solution to the problem.
## What it does
We created an algorithm to find the possible valid floor plans for the building based on the challenge parameters. Then the algorithm found the best floor plan according to team preferences and maximizing occupied office space. The output was displayed to the console depicting which teams should be put on each floor.
## How we built it
We found the valid floor plans with a depth first search algorithm implemented in C++. We chose C++ because of our prior experience with it and because of its efficiency. We also created a ranking algorithm in C++ to find which floor plan was optimal by looking at team preferences and each team's dependencies (how many teams preferred them).
## Challenges we ran into
The biggest problem was with the depth first search algorithm not working properly. After a lot of debugging, the issue was that during backtracking, teams were being popped out which disrupted the rest of the outputs.
## Accomplishments that we're proud of
We're proud that we found a working solution to the challenge and that we successfully implemented the algorithms in C++ with only a little prior experience. For two of our teammates, this was their first hackathon.
## What we learned
We learned a lot about object oriented programming in C++, depth first search algorithms, and recursion while working on this project.

To run the file, compile main.cpp and follow instructions.
