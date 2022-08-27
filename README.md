# Socket_Programming
Socket Programming on one local host and several ports. The clients connect with the server through TCP connections.


Each message is consisted of 4 parts: message type, message ID, length, and payload.

There are 9 different message types, each with a different ID, which enables TCP connections between different messages. 

Length section indicates the entire length of a message. 

After the client connects to the server, it will send its name through CONNECT to the server. The server will send CONNACK in respond to that message. 

Each client can ask for a complete list of its users through LIST. Server will send that list to the client through LISTREPLY. 

Each client can send the ID of a user through INFO, and ask for the user's name from the server. The server will answer through INFOREPLY, where the name of the user is stored in the payload section.

Client will use SEND to send a message, which contains the ID of the user that the message needs to be sent to, and the content of the message. The server will answer through SENDREPLY.

Client will use RECIEVE to recieve the messages that have been sent to it. Server will answer through RECIEVEREPLY. 

EXIT is used to terminate the program.
