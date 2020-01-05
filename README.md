# flightsimulator

# Flight simulator interpreter

Flight simulator interpreter is a C++ library which allows users to program actions into the flight simulator and run them.

## Installation of the flight simulator

Use the official website [flight simulator](http://home.flightgear.org) to install application to your computer.

Use the command line to set the basic settings such as:
```bash
#To open a server from the simulator on the requested port and ip.
--telnet=socket,in,10,127.0.0.1,5402,tcp
#To open a web server on port 8080.
--httpd=8080
```
for more information about controlling the simulator visit [flight gear wiki](http://wiki.flightgear.org/Main_Page).

## Interpreter code structure

```bash
1. openDataServer(5400) //blockingcall
2. connectControlClient("127.0.0.1",5402) // blocking call
3. varbreaks->sim("/controls/flight/speedbreak")
4. varthrottle->sim("/controls/engines/current-engine/throttle")
5. varheading<-sim("/instrumentation/heading-indicator/offset-deg")
6. var airspeed<-sim("/instrumentation/airspeed-indicator/indicated-speed-kt")
7. varroll<-sim("/instrumentation/attitude-indicator/indicated-roll-deg")
8. varpitch<-sim("/instrumentation/attitude-indicator/internal-pitch-deg")
9. varrudder<-sim("/controls/flight/rudder")
10. var aileron <- sim("/controls/flight/aileron")
11. var elevator <- sim("/controls/flight/elevator")
12. var alt <- sim("/instrumentation/altimeter/indicated-altitude-ft")
13. breaks = 0
14. throttle = 1
15. var h0 = heading
16. if alt < 1000 {
17.   rudder = (h0 – heading)/20
18.   aileron = - roll / 70
19.   elevator = pitch / 50
20.   Print(alt)
21.   Sleep(250)
22. }
23. Print("start takeoff")
24. takeoff(var x) {
25.   Print(x)
26.   while alt < x {
27.       rudder = (h0 - heading)/80
28.       aileron = -roll / 70
29.       elevator = pitch / 50
30.       Print(alt)
31.       Sleep(250)
32.   }
33. }
34. takeoff(1000)
35. Print("done")
```
#### Variables
As you can see in the example, we can assign variables to only get data from the simulator using <-,
or also update their value inside the simulator using ->.

We can also assign local variables that aren't connected to the simulator itself using regular var x = 'value'.
Every variable declaration has to start with the var keyword.

When declaring a variable inside function / if / while it will created as a scoped variable, such variables are only accessible by higher scopes or themselves and will be deleted after we leave their scope.

#### Print
Use the keyword Print(variable / string value) to print something to the screen, use "string" to print regular string, and don't use varName in
#### Sleep
Use the keyword Sleep(integer value) to make the main thread sleep for the requested amount of time in milliseconds.
#### Functions
Declare a method by calling methodName(var varName1, ...) { method content }

Run the method by calling existingMethodName(var1Value, ...)
The values sent to a method are Double or existing variables.
#### While loop
Declare a while loop by calling while e1 [CONDITION] e2 { loop content }
#### If condition
Declare an if condition by calling if e1 [CONDITION] e2 { if content }

## Client - Server flow of the simulator
the code of the interpreter should always start with the commands:

```bash
1. openDataServer(5400)
2. connectControlClient("127.0.0.1",5402)
```

### openDataServer
this command opens a thread in which we open and run a tcp server. Our simulator connects to the server as client and sends requests. Each requests contain the values of the airplane. Our server accepts the requests and updates the database accordingly.

### connectControlClient
this command opens a thread in which we open and run a tcp client. Now, we act as a client who sends requests to the simulator. Every time we get a new command for the simulator from the code we are sending it to the simulator as a request through this client.

**Both of these commands acts as blocking call meaning, we are locking the main thread from running until the server client gets the first request from the simulator and the client sent his first request to update the values I the simulator.**

## Interpreting the code
in order to get the program running we need to be able to interpret the code. we have to parts to this section of the program:

### Lexer
The ```Lexer``` class has a unique function:

```c++
static vector<string> lexerCode(std::string filename);
```
This function splits the code to tokens and return a vector containing the tokens.

#### For example:
if the file would contain the lines:
```bash
1. openDataServer(5400)
2. connectControlClient("127.0.0.1",5402)
```
The values ```lexerCode() ```  would return will be:

```openDataServer, 5400, connectControlClient, 127.0.0.1, 5402```

### Parser
The ```Parser``` class iterate over the vector given by  the ```Lexer``` and perform a command each time it finds one.
 
### Command
The ```Command``` class and subclasses hold the actual actions that we will run. The commands are run from the parser.

### Run the project:
1. Create a code file with commands such as the one above (ports and ip should always be the same as the example code).
2. Compile the code using:
```
g++ -std=c++14 *.cpp -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o a.out -pthread
```
3. Run a.out with the .txt code file as an argument:
```
.\a.out fly.txt
```
4. Run the simulator after the project is running.


Enjoy!
