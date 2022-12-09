## Instructions on how to run.

You need to create the server and the client binary files

1. cd to the parent directory
2. Run `gcc webClient.c -o client -lpthread` to compile the file for the client
3. Run `gcc webServer.c -o server -lpthread` to compile the file for the server
4. On your terminal run `./server` which will create the server that will be listening to the connection from the clients
5. On your terminal cd open up different windows that will allow you to create different clients that will make the calls / conections to the server. while here run `./client` and follow the prompts.
6. Go back to the terminal with the client to observe the connections.
