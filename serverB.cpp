#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <limits>
#include <fstream>
#include <bitset>
#include <unistd.h>
#include <random>
#include <pthread.h>
#include <vector>



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

void* serverThreads(void *sVec){

  std::cout << "Z";

  std::random_device rd;     // initialise random (seed) engine
  std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  std::uniform_int_distribution<int> uni(1,5); // guaranteed unbiased
  std::string request = "messageFile";


  std::string reply = "ACK";

    std::vector <ServerSocket>test = *(std::vector<ServerSocket>*) sVec;


  ServerSocket sendSock = test[0];

    ServerSocket recSock = test[1];

    recSock >> request;

std::string message = readFile (request);


    try {
        unsigned int MESSAGE_SIZE = 7 * 71; // 7 * 71 = 497
        unsigned int frameSentCounter = 0;
        std::string encodedMessage = encode(message, false);
        for (int i = 0; i < encodedMessage.size(); i+= MESSAGE_SIZE) {
          do {
            frameSentCounter++;
            std::string currentFrame = encodedMessage.substr(i, MESSAGE_SIZE);
            int random_integer = uni(rng); //rand int 1-5

            if (random_integer % 5 == 0) { // send wrong parity every 5 frames // CHANGE TO RANDOM!!!!!
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


            std::cout << "Q";
          } while (reply != "ACK");
          std::cout << std::endl;
        }

    }
    catch (SocketException&){
    }
 pthread_exit(0);
}


int main(int argc, int argv[])
{

  try{
      // Create the socket
unsigned int threadCount =0; //numeric amount of threads running
std::string countClient="zero"; //string to send to client as ints are not an option
      pthread_t td[4]; // container for the threads

      //all the socket creation done this way so when a thread is doing stuff
      //the main does not comback and over write serversend with new info
      //as we pass things by with pointers
      std::vector<ServerSocket> socketsVector1 (2); //creating stuff for thread 1
      ServerSocket serverSend1(30001);
        ServerSocket serverRec1(30002);
        ServerSocket sSend1;
        ServerSocket sRec1;

std::vector<ServerSocket> socketsVector2 (2);
        ServerSocket serverSend2(30003);
          ServerSocket serverRec2(30004);
          ServerSocket sSend2;
          ServerSocket sRec2;

std::vector<ServerSocket> socketsVector3 (2);
          ServerSocket serverSend3(30005);
            ServerSocket serverRec3(30006);
            ServerSocket sSend3;
            ServerSocket sRec3;

std::vector<ServerSocket> socketsVector4 (2);
            ServerSocket serverSend4(30007);
              ServerSocket serverRec4(30008);
              ServerSocket sSend4;
              ServerSocket sRec4;

std::vector<ServerSocket> socketsVector5 (2);
              ServerSocket serverSend5(30009);
                ServerSocket serverRec5(30010);
                ServerSocket sSend5;
                ServerSocket sRec5;
      while (true){
        ServerSocket serverWait(30000); // to catch all incoming requests
        ServerSocket sWait;
        serverWait.accept(sWait);
        sWait << countClient; //tell them what thread we on
        switch (threadCount) {
          case 0: //case to start the right thread
            threadCount++; //incrementing to keep track of things
            serverRec1.accept(sRec1);
            serverSend1.accept(sSend1);
            socketsVector1[0] =sSend1; // putting them in the vector to be split in threads
            socketsVector1[1]=sRec1;

            pthread_create(&td[threadCount], NULL, serverThreads, (void *) &socketsVector1);
// creating the first thread
            countClient="one";
            break;
          case 1:
            serverRec2.accept(sRec2);
            serverSend2.accept(sSend2);
            socketsVector2[0] =sSend2;
            socketsVector2[1]=sRec2;
            pthread_create(&td[threadCount], NULL, serverThreads, (void *) &socketsVector2);
            threadCount++;
            countClient ="two";
            break;
          case 2:
            serverRec3.accept(sRec3);
            serverSend3.accept(sSend3);
            socketsVector3[0] =sSend3;
            socketsVector3[1]=sRec3;
            pthread_create(&td[threadCount], NULL, serverThreads, (void *) &socketsVector3);
            threadCount++;
            countClient="three";
            break;
          case 3:
            serverRec4.accept(sRec4);
            serverSend4.accept(sSend4);
            socketsVector4[0] =sSend4;
            socketsVector4[1]=sRec4;
            pthread_create(&td[threadCount], NULL, serverThreads, (void *) &socketsVector4);
            threadCount++;
            countClient="four";
            break;
            case 4:
              serverRec5.accept(sRec5);
              serverSend5.accept(sSend5);
              socketsVector5[0] =sSend5;
              socketsVector5[1]=sRec5;
              pthread_create(&td[threadCount], NULL, serverThreads, (void *) &socketsVector5);
              pthread_join(td[0], NULL);
              pthread_join(td[1], NULL);
              pthread_join(td[2], NULL);
              pthread_join(td[3], NULL);
              pthread_join(td[4], NULL);
              //all 5 concurent ones are done so kill them all
              threadCount = 0;
              countClient = "zero";
              //reset counters so 5 at once and no cap
              break;
          }


      }


  }
  catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }

  return 0;
}
