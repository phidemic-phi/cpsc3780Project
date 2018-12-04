
#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <unistd.h>
#include <limits>
#include <cmath>


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

int convertBinaryToDecimal(long long n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

int stoi( std::string message){
   int wholeNumber =0;
   char letter;
   for(int i= 0; i<7;i++){
      letter = message[i];
      if(letter == '0')
	 wholeNumber+0;
      else if(letter == '1')
	 wholeNumber + 1;
      wholeNumber*10;
   }
   wholeNumber= convertBinaryToDecimal(wholeNumber);
   return wholeNumber;
}


std::string decode(std::string msg) {
   std::string ASCIIMsg = "";
   char holder;
  for (int i = 0; i < msg.size(); i+=7) {
     if (msg.substr(i, 7).size() >= 7)
	holder = stoi(msg.substr(i,7));
     ASCIIMsg += holder;
    }
  return ASCIIMsg;
}

int main(int argc, int argv[])
{

  bool debuggingText = false;
  try{
    int threadCount;
    std::string caseClient, request;
    ClientSocket clientConnect ("Network", 30000);
    clientConnect >> caseClient; // couldnt send int's needed string
    if (caseClient == "zero") //strings cant do cases
        threadCount =1;
        else if (caseClient == "one")
        threadCount =3;
        else if (caseClient == "two")
        threadCount = 5;
      else if (caseClient == "three")
        threadCount = 7;
      else if (caseClient == "four")
        threadCount = 9;

      ClientSocket clientRec ("Network", 30000+threadCount);
      ClientSocket clientSend ("Network", 30001+threadCount);
      std::string reply;
      std::cin >> request;
      try {
          clientSend << request; //requesting a specific file no .txt needed
  	      while (true){

  		        std::string data;

  		        clientRec >> data;
			std:: cout << data;
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


              if (parityIsOdd(data) == false) { //parityIsOdd(data) == false

                clientSend << "ACK";
                if (debuggingText) {std::cout << "Sending back ACK. Contents:\n'";}
                std::cout << decode(data) <<std::endl;
               }
              else {

                clientSend << "NACK";
              if (debuggingText)
                std::cout << "<< Sending back NAK. >>\n";
                std::cout << decode(data) <<std::endl;
              }


  	      }

  	  }

  	  catch(SocketException&){
            }

  	  }

      catch ( SocketException& e ){
          std::cout << "Exception was caught:" << e.description() << "\n";
      }

  return 0;
}
