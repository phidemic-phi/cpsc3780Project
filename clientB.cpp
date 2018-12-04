#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <unistd.h>
#include <limits>


bool parityIsOdd (std::string msg) {
  unsigned int numEven = 0;

  for (int i = 0; i < msg.size(); i++) {
    if (int(msg[i]) == 49) {
      numEven++;
    }
  }

  // if number of  bits is odd:
  if (numEven % 2) {
    return true;
    }
  else { return false; }
}

std::string encode(std::string msg, bool causeError) {
  std::string binaryMsg = "";
  for (int i = 0; i < msg.size(); i++) {
    std::string binary = std::bitset<7>(int(msg[i])).to_string();
    binaryMsg += binary;
  }
  return binaryMsg;
}

std::string decode(std::string msg) {
  std::string ASCIIMsg = "";
  for (int i = 0; i < msg.size(); i+=7) {
    if (msg.substr(i, 7).size() >= 7)
      ASCIIMsg += char(std::stoi(msg.substr(i, 7), nullptr, 2));
    }
  return ASCIIMsg;
}

std::string readFile(std::string fileName) {
  std::string output;
  std::string line;
  std::string fullFileName = fileName + ".txt";
  std::ifstream myfile (fullFileName);

  if (myfile.is_open()) {
    while (getline (myfile,line)) {
      output += line;
    }
    myfile.close();
  }
  else std::cout << "Unable to open file\n";

  return output;
}


int main(int argc, int argv[])
{

  bool debuggingText = true;
  try{
    //  ClientSocket clientSend ("Network", 30001);
    //std:: cout << "attept1";
      ClientSocket clientRec ("Network", 30000);
      usleep(100);
      //return 0;
    //  std:: cout << "attept 2";
      ClientSocket clientSend ("Network", 30001);
      std::string reply;

      try {



          //clientSend << "Hello";
            //clientRec << "Hello";
            usleep(100);
clientRec >> reply;
std:: cout << reply;
            //clientSend << "hello";
          std::cout << "H";

          clientSend << "messageFile";
  	      while (true){

             std::cout << "L";
  		        std::string data;
usleep(10000);
  		        clientRec >> data;
              if (debuggingText)
                std::cout << "\nReceived frame. " << data;
              if (!parityIsOdd(data)) {
                if (debuggingText)
                  std::cout << "Status: OK." << std::endl;
              }
              else {
                if (debuggingText)
                  std::cout << "Status: ERROR DETECTED." << std::endl;
              }

              std::cout << "O";
              if (parityIsOdd(data) == false) { //parityIsOdd(data) == false
                std::cout << "V";
                clientSend << "ACK";
                if (debuggingText) {std::cout << "Sending back ACK. Contents:\n'";}
                std::cout << decode(data) <<std::endl;
               }
              else {
                std::cout << "D";
                clientSend << "NACK";
              if (debuggingText)
                std::cout << "<< Sending back NAK. >>\n";
              }
            //std:: cout<< true;

  	      }
          //std:: cout << "left input loop";
  	  }

  	  catch(SocketException&){
            }

  	  }

      catch ( SocketException& e ){
          std::cout << "Exception was caught:" << e.description() << "\n";
      }

  return 0;
}
