#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "message.h"

#define List "list";
#define Info "info";
#define Send "send";
#define Recieve"receive";
#define Exit "exit";

using namespace std;

void list_input(Header to_server, Header to_client, int fd){
  to_server.message_type = List;
  to_server.message_id = List;
  to_server.length = 2;
  write(fd, &to_server, sizeof(to_server));
  read(fd, &to_client, sizeof(to_client));
  uint16_t buffer;
  for (int i = 0; i < (to_client.length - sizeof(Header)); i = i + 2)
  {
    read(fd, (uint8_t*)&buffer, sizeof(buffer));
    cout << buffer << endl;
  }
}

void Info_input(Header to_server, Header to_client, int fd){
  string name;
  uint16_t ID;
  cin >> ID;
  to_server.message_type = Info;
  to_server.message_id = Info;
  to_server.length = 4;

  write(fd, &to_server, sizeof(to_server));
  write(fd, &ID, sizeof(ID));
  read(fd, &to_client, sizeof(to_client));
  cout << unsigned(to_client.message_type) << endl;
  read(fd, (uint8_t*)name.c_str(), name.length());
  cout << name << endl;
}

void Send_input(Header to_server, Header to_client, int fd){
  string ID, input;
  cin >> ID, input;
  to_server.message_type = Send;
  to_server.message_id = Send;
  to_server.length = 4 + input.length();
  write(fd, &to_server, sizeof(to_server));
}

void Recieve_input(Header to_server, Header to_client, int fd){
  to_server.message_type = Recieve;
  to_server.message_id = Recieve;
  to_server.length = 2;
  write(fd, &to_server, sizeof(to_server));
  read(fd, &to_client, sizeof(to_client));
}

vector<string> seperate_data (string input_str, char divider)
{
    vector <string> strings;
    int index_curr = 0;
    int index_start = 0;
    int index_end = 0;
    while (index_end <= input_str.length())
    {
      if (input_str[index_end] == divider || index_end == input_str.length())
      {
        string sub_str = "";
        int difference = index_end - index_start;
        sub_str.append(input_str, index_start, difference);
        strings.push_back(sub_str);
        index_curr++;
        index_start = index_end + 1;
      }
      index_end++;
    }
    return strings;
}

int main(int argc, char* argv[])
{
  string port(argv[1]);
  string name(argv[2]);
  vector<string> seperated = seperate_data(port, ':');
  cout << stoi(seperated[1]) << endl;
  int port = stoi(seperated[1]);
  int len;
  string command;
  vector<string> seperated_str;

  auto address_server = sockaddr_in();
  address_server.sin_family = AF_INET;
  address_server.sin_port = htons(port);
  auto err = inet_pton(AF_INET, HOST_IP, &address_server.sin_addr);
  if (err < 0)
  {
    cerr << "failed to complete address " << err << endl;
    return EXIT_FAILURE;
  }
  auto fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
  {
    cerr << "failed to create socket " << fd << endl;
    goto wait;
  }
  err = connect(fd, (sockaddr*)&srv_addr, sizeof(srv_addr));
  if (err < 0)
  {
    cerr << "failed to connect" << err << endl;
    goto exit;
  }
  cout << "connected to " << "127.0.0.1" << ":" << port << endl;

  Header to_server;
  Header to_client;
  to_server.message_type = CONNECT;
  to_server.message_id = CONNECT;
  to_server.length = 2 + len(name);

  write(fd, &to_server, sizeof(to_server));
  write(fd, name.c_str(), strlen(name.c_str()));
  read(fd, &to_client, sizeof(to_client));

  while(cin >> command)
  {
    if (command == List)
    {
      list_input(Header to_server, Header to_client, int fd);
    }
    else if (command == Info)
    {
      Info_input(Header to_server, Header to_client, int fd);
    }
    else if(command == Send)
    {
      Send_input(Header to_server, Header to_client, int fd);
    }
    else if(command == Recieve)
    {
      Recieve_input(Header to_server, Header to_client, int fd);
    }
    else if(command == Exit)
    {
      goto exit;
    }
    exit:
      close(fd);

    wait:
      sleep(1);
  }
    return EXIT_SUCCESS;
}

