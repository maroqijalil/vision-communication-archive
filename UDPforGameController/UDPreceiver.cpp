#include <iostream>
#include <string>
#include <string.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/socket_base.hpp>
#include <boost/asio/basic_datagram_socket.hpp>
#include <cstdio>

namespace b1 = boost::asio;
namespace b2 = boost::system;
using boost::array;
using b1::ip::udp;
using std::cout;
using std::endl;
using std::string;
using std::cerr;

#define GAMECONTROLLER_STRUCT_HEADER   "RGme"
#define GAMECONTROLLER_STRUCT_VERSION  8

#define SPL_COACH_MESSAGE_SIZE      40
#define MAX_NUM_PLAYERS             11

#define TEAM_BLUE                   0
#define TEAM_CYAN                   0
#define TEAM_RED                    1
#define TEAM_MAGENTA                1
#define DROPBALL                    2

#define STATE_INITIAL               0
#define STATE_READY                 1
#define STATE_SET                   2
#define STATE_PLAYING               3
#define STATE_FINISHED              4

#define STATE2_NORMAL               0
#define STATE2_PENALTYSHOOT         1
#define STATE2_OVERTIME             2
#define STATE2_TIMEOUT              3

#define PENALTY_NONE                        0
// SPL
#define PENALTY_SPL_BALL_HOLDING            1
#define PENALTY_SPL_PLAYER_PUSHING          2
#define PENALTY_SPL_OBSTRUCTION             3
#define PENALTY_SPL_INACTIVE_PLAYER         4
#define PENALTY_SPL_ILLEGAL_DEFENDER        5
#define PENALTY_SPL_LEAVING_THE_FIELD       6
#define PENALTY_SPL_PLAYING_WITH_HANDS      7
#define PENALTY_SPL_REQUEST_FOR_PICKUP      8
#define PENALTY_SPL_COACH_MOTION            9
// HL Kid Size
#define PENALTY_HL_KID_BALL_MANIPULATION    1
#define PENALTY_HL_KID_PHYSICAL_CONTACT     2
#define PENALTY_HL_KID_ILLEGAL_ATTACK       3
#define PENALTY_HL_KID_ILLEGAL_DEFENSE      4
#define PENALTY_HL_KID_REQUEST_FOR_PICKUP   5
#define PENALTY_HL_KID_REQUEST_FOR_SERVICE  6
#define PENALTY_HL_KID_REQUEST_FOR_PICKUP_2_SERVICE 7
// HL Teen Size
#define PENALTY_HL_TEEN_BALL_MANIPULATION   1
#define PENALTY_HL_TEEN_PHYSICAL_CONTACT    2
#define PENALTY_HL_TEEN_ILLEGAL_ATTACK      3
#define PENALTY_HL_TEEN_ILLEGAL_DEFENSE     4
#define PENALTY_HL_TEEN_REQUEST_FOR_PICKUP  5
#define PENALTY_HL_TEEN_REQUEST_FOR_SERVICE 6
#define PENALTY_HL_TEEN_REQUEST_FOR_PICKUP_2_SERVICE 7

#define PENALTY_SUBSTITUTE                  14
#define PENALTY_MANUAL                      15

#pragma pack (1)
typedef struct
{
  uint8_t penalty;              // penalty state of the player
  uint8_t secsTillUnpenalised;  // estimate of time till unpenalised
} RobotInfo;
#pragma pack(pop)

#pragma pack (1)
typedef struct
{
  uint8_t teamNumber;           // unique team number
  uint8_t teamColour;           // colour of the team
  uint8_t score;                // team's score
  uint8_t penaltyShot;          // penalty shot counter
  uint16_t singleShots;         // bits represent penalty shot success
  uint8_t coachMessage[SPL_COACH_MESSAGE_SIZE]; // the coach's message to the team
  RobotInfo coach;
  RobotInfo players[MAX_NUM_PLAYERS]; // the team's players
} TeamInfo;
#pragma pack(pop)

#pragma pack (1)
typedef struct
{
  uint32_t header;               // header to identify the structure
  uint8_t version;              // version of the data structure
  uint8_t packetNumber;         // number incremented with each packet sent (with wraparound)
  uint8_t playersPerTeam;       // The number of players on a team
  uint8_t state;                // state of the game (STATE_READY, STATE_PLAYING, etc)
  uint8_t firstHalf;            // 1 = game in first half, 0 otherwise
  uint8_t kickOffTeam;          // the next team to kick off (TEAM_BLUE, TEAM_RED)
  uint8_t secondaryState;       // Extra state information - (STATE2_NORMAL, STATE2_PENALTYSHOOT, etc)
  uint8_t dropInTeam;           // team that caused last drop in
  uint16_t dropInTime;          // number of seconds passed since the last drop in.  -1 before first dropin
  uint16_t secsRemaining;       // estimate of number of seconds remaining in the half
  uint16_t secondaryTime;       // number of seconds shown as secondary time (remaining ready, until free ball, etc)
  TeamInfo teams[2];
} RoboCupGameControlData, *pRoboCupGameControlData;
#pragma pack(pop)

int main()
{
  try
  {
    b1::io_service io;
    udp::endpoint local_endpoint;
    udp::endpoint remote_endpoint;
    udp::socket sock(io);
    boost::asio::socket_base::reuse_address option(true);
    
    sock.open(udp::v4());
    sock.set_option(option);

    remote_endpoint = udp::endpoint(b1::ip::address_v4::any(), 3838);
    local_endpoint = udp::endpoint(b1::ip::address_v4::any(), 3838);
    sock.bind(local_endpoint);

    for (;;)
    {
      
      // array<char, 1024> pesank;
      uint8_t pesank[200];
      // udp::endpoint endpointk;
      b2::error_code perror;
      // i++;
      // sock.get_option(option);
      // bool is_set = option.value();

      size_t panjang;

      panjang = sock.receive_from(b1::buffer(pesank), remote_endpoint, 0, perror);
      // if(perror == b1::error::eof)
      //   break;

      pRoboCupGameControlData converted_m = (pRoboCupGameControlData)pesank;

      // char buffer[panjang][5];
      // for(int i = 0 ; i < panjang ; i++)
      // {
      //   sprintf(buffer[i], "%02X", pesank[i]);
      // }

      if(converted_m->version == GAMECONTROLLER_STRUCT_VERSION){
        // cout.write(pesank.data(), panjang);
        // cout << panjang << endl;
        // cout << *buffer[7] << endl;
        
        cout << "Primary State: ";
        switch (converted_m->state)
        {
          case STATE_INITIAL: cout << "Initial" << endl; break;
          case STATE_READY: cout << "Ready" << endl; break;
          case STATE_SET: cout << "Set" << endl; break;
          case STATE_PLAYING: cout << "Play" << endl; break;
          case STATE_FINISHED: cout << "Finish" << endl; break;
          default: cout << "None" << endl; break;
        }

        cout << "Secondary State: ";
        switch (converted_m->secondaryState)
				{
					case STATE2_NORMAL: cout << "Normal" << endl; break;
					case STATE2_PENALTYSHOOT: cout << "Penalty Shoot" << endl; break;
					case STATE2_OVERTIME: cout << "Overtime" << endl; break;
					case STATE2_TIMEOUT: cout << "Timeout" << endl; break;
					default: cout << "None" << endl; break;
				}

        cout << "Team Data:" << endl;
        
        switch (converted_m->teams[0].teamColour)
        {
          case TEAM_BLUE: cout << "  Blue Team Score: " << (int)converted_m->teams[0].score << endl; break;
          case TEAM_RED: cout << "  Red Team Score: " << (int)converted_m->teams[0].score << endl; break;
          default: cout << "None" << endl; break;
        }
        for (int i = 0; i < MAX_NUM_PLAYERS; i++)
        {
          switch (converted_m->teams[0].players[i].penalty)
          {
            case PENALTY_HL_TEEN_BALL_MANIPULATION: cout << "   Player " << i+1 << ": Ball Manipulation" << endl; break;
            case PENALTY_HL_TEEN_PHYSICAL_CONTACT: cout << "   Player " << i+1 << ": Physical Contact" << endl; break;
            case PENALTY_HL_TEEN_ILLEGAL_ATTACK: cout << "   Player " << i+1 << ": Illegal Attack" << endl; break;
            case PENALTY_HL_TEEN_ILLEGAL_DEFENSE: cout << "   Player " << i+1 << ": Illegal Defense" << endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_PICKUP: cout << "   Player " << i+1 << ": Request for Pickup" << endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_SERVICE: cout << "   Player " << i+1 << ": Request for Service" << endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_PICKUP_2_SERVICE: cout << "   Player " << i+1 << ": Request for Pickup from Service" << endl; break;
            default: break;
          }
        }

        switch (converted_m->teams[1].teamColour)
        {
          case TEAM_BLUE: cout << "  Blue Team Score: " << (int)converted_m->teams[1].score << endl; break;
          case TEAM_RED: cout << "  Red Team Score: " << (int)converted_m->teams[1].score << endl; break;
          default: cout << "None" << endl; break;
        }
        for (int i = 0; i < MAX_NUM_PLAYERS; i++)
        {
          switch (converted_m->teams[1].players[i].penalty)
          {
            case PENALTY_HL_TEEN_BALL_MANIPULATION: cout << "   Player " << i+1 << ": Ball Manipulation" << endl; break;
            case PENALTY_HL_TEEN_PHYSICAL_CONTACT: cout << "   Player " << i+1 << ": Physical Contact" << endl; break;
            case PENALTY_HL_TEEN_ILLEGAL_ATTACK: cout << "   Player " << i+1 << ": Illegal Attack" << endl; break;
            case PENALTY_HL_TEEN_ILLEGAL_DEFENSE: cout << "   Player " << i+1 << ": Illegal Defense" << endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_PICKUP: cout << "   Player " << i+1 << ": Request for Pickup" << endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_SERVICE: cout << "   Player " << i+1 << ": Request for Service" << endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_PICKUP_2_SERVICE: cout << "   Player " << i+1 << ": Request for Pickup from Service" << endl; break;
            default: break;
          }
        }

        cout << endl;
      }

      if (perror && perror != b1::error::message_size)
        throw b2::system_error(perror);
      
    }
  }
  catch (std::exception& e)
  {
    cerr << e.what() << endl;
  }

  return 0;
}