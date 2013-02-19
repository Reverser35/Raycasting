After messing around with euclidean angle ray casting I found a more straightforward explanation of ray casting using vectors here: http://lodev.org/cgtutor/raycasting.html. Adding some simple collision detection to the method presented there led to this project. In its current state it's a lightweight raycaster written in C and standard SDL. It requires the SDL framework but doesn't require the additional SDL codebase that the mentioned tutorial uses. Compiles under Xcode 4.5 and Mac OS X 10.8.