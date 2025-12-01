This program solves a Rubik's cube using the meet-in-the-middle algorithm.

It performs two searches:
1. A "forward" search from the solved state.
2. A "backward" search from the scrambled state.

When the two searches find a common cube state, a solution is found by combining the paths.

Usage: `./<executable_name> <forward_depth> <backward_depth>`

Example: `./a.out 7 8`