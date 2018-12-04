#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <limits>
#include <fstream>
#include <bitset>
#include <unistd.h>
//#include <random>
#include <pthread.h>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


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
/*
std::string decode(std::string msg) {
  std::string ASCIIMsg = "";
  for (int i = 0; i < msg.size(); i+=7) {
    if (msg.substr(i, 7).size() >= 7)
      ASCIIMsg += char(std::stoi(msg.substr(i, 7), nullptr, 2));
  }
  return ASCIIMsg;
}
*/
std::string readFile(std::string fileName) {
 std::string output;
  std::string line;
  std::ifstream myfile;
  if (fileName == "1")
     myfile.open("messageFile.txt", std::ifstream::in);
 
  if (myfile.is_open()) {
    while (getline (myfile,line)) {
      output += line;
    }
    myfile.close();
  }
  else std::cout << "Unable to open file\n";

  return output;
}


///////////////////////////////////////////////////////////////////////////////
int main(int argc, int argv[])
{
   srand(time(NULL));
   try{


      ServerSocket serverSend(30001);
      ServerSocket serverRec(30002);
      ServerSocket sendSock;
      ServerSocket recSock;


      while (true){
	 ServerSocket server (30000);
	 ServerSocket sWait;
	 server.accept(sWait);
	 sWait << "zero";
	 serverRec.accept(recSock);
	 serverSend.accept(sendSock);
	 std::string request = "messageFile";
	 std::string reply = "ACK";
	 /////////////////////////////////////
	 recSock >> request; // RECIEVE FILENAME
	 std::cout << "(1) RECEIVE FILENAME: " << request << std::endl; 
	 /////////////////////////////////////
	 std::string message = readFile (request);
	 try {
	    unsigned int MESSAGE_SIZE = 7 * 71; // 7 * 71 = 497
	    unsigned int frameSentCounter = 0;
	    std::string encodedMessage = encode(message, false);
	    for (int i = 0; i < encodedMessage.size(); i+= MESSAGE_SIZE) {
	       do {
		  frameSentCounter++;
		  std::string currentFrame = encodedMessage.substr(i, MESSAGE_SIZE);
           

		  if (rand() % 5 == 0) { // send wrong parity every 5 frames // CHANGE TO RANDOM!!!!!
		     if (parityIsOdd(currentFrame)) { currentFrame += "0"; } //WRONG
		     else { currentFrame += "1"; } //WRONG
		  }
		  else{
		     if (parityIsOdd(currentFrame)) { currentFrame += "1"; } //CORRECT
		     else { currentFrame += "0"; } //CORRECT
		  }

		  sendSock << currentFrame;

		  std::cout << "Frame sent." << std::endl;
		  recSock >> reply;

		  if (reply == "ACK") {std::cout << "Received ACK.\n";}
		  else if (reply != "ACK") {std::cout << "Received NACK, retransmitting.\n";}



	       } while (reply != "ACK");
	       std::cout << std::endl;
	    }

	 }
	 catch (SocketException&){
	 }
         
      	    
      
         

      }
   }
   catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
   return 0;
}
