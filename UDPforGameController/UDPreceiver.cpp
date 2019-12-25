#include "boost/array.hpp"
#include "boost/asio.hpp"
#include "boost/asio/basic_datagram_socket.hpp"
#include "boost/asio/socket_base.hpp"

#include <cstdio>
#include <iostream>
#include <string>
#include <string.h>

using namespace boost::asio;
using namespace boost::system;
using boost::asio::ip::udp;

enum
{
  GAMECONTROLLER_STRUCT_VERSION = 8
};

enum
{
  SPL_COACH_MESSAGE_SIZE = 40,
  MAX_NUM_PLAYERS = 11
};

enum
{
  TEAM_BLUE = 0,
  TEAM_CYAN = 0,
  TEAM_RED = 1,
  TEAM_MAGENTA = 1,
  DROPBALL = 2
};

enum
{
  STATE_INITIAL = 0,
  STATE_READY = 1,
  STATE_SET = 2,
  STATE_PLAYING = 3,
  STATE_FINISHED = 4
};

enum
{
  STATE2_NORMAL = 0,
  STATE2_PENALTYSHOOT = 1,
  STATE2_OVERTIME = 2,
  STATE2_TIMEOUT = 3
};

enum
{
  PENALTY_NONE = 0,
  
  // SPL
  PENALTY_SPL_BALL_HOLDING = 1,
  PENALTY_SPL_PLAYER_PUSHING = 2,
  PENALTY_SPL_OBSTRUCTION = 3,
  PENALTY_SPL_INACTIVE_PLAYER = 4,
  PENALTY_SPL_ILLEGAL_DEFENDER = 5,
  PENALTY_SPL_LEAVING_THE_FIELD = 6,
  PENALTY_SPL_PLAYING_WITH_HANDS = 7,
  PENALTY_SPL_REQUEST_FOR_PICKUP = 8,
  PENALTY_SPL_COACH_MOTION = 9
};

// HL Kid Size
enum
{
  PENALTY_HL_KID_BALL_MANIPULATION = 1,
  PENALTY_HL_KID_PHYSICAL_CONTACT = 2,
  PENALTY_HL_KID_ILLEGAL_ATTACK = 3,
  PENALTY_HL_KID_ILLEGAL_DEFENSE = 4,
  PENALTY_HL_KID_REQUEST_FOR_PICKUP = 5,
  PENALTY_HL_KID_REQUEST_FOR_SERVICE = 6,
  PENALTY_HL_KID_REQUEST_FOR_PICKUP_2_SERVICE = 7
};

// HL Teen Size
enum
{
  PENALTY_HL_TEEN_BALL_MANIPULATION = 1,
  PENALTY_HL_TEEN_PHYSICAL_CONTACT = 2,
  PENALTY_HL_TEEN_ILLEGAL_ATTACK = 3,
  PENALTY_HL_TEEN_ILLEGAL_DEFENSE = 4,
  PENALTY_HL_TEEN_REQUEST_FOR_PICKUP = 5,
  PENALTY_HL_TEEN_REQUEST_FOR_SERVICE = 6,
  PENALTY_HL_TEEN_REQUEST_FOR_PICKUP_2_SERVICE = 7
};

enum
{
  PENALTY_SUBSTITUTE = 14,
  PENALTY_MANUAL = 15
};

class RobotInfo
{
public :
  uint8_t penalty;
  uint8_t secsTillUnpenalised;
};

class TeamInfo
{
public :
  uint8_t teamNumber;
  uint8_t teamColour;
  uint8_t score;
  uint8_t penaltyShot;
  uint16_t singleShots;
  uint8_t coachMessage[SPL_COACH_MESSAGE_SIZE];
  RobotInfo coach;
  RobotInfo players[MAX_NUM_PLAYERS];
};

class pRoboCupGameControlData
{
public :
  uint32_t header;
  uint8_t version;
  uint8_t packetNumber;
  uint8_t playersPerTeam;
  uint8_t state;
  uint8_t firstHalf;
  uint8_t kickOffTeam;
  uint8_t secondaryState;
  uint8_t dropInTeam;
  uint16_t dropInTime;
  uint16_t secsRemaining;
  uint16_t secondaryTime;
  TeamInfo teams[2];
};

int main()
{
  try
  {
    boost::asio::io_service io;
    udp::endpoint local_endpoint;
    udp::endpoint remote_endpoint;
    udp::socket socket_(io);
    boost::asio::socket_base::reuse_address option(true);
    
    socket_.open(udp::v4());
    socket_.set_option(option);

    remote_endpoint = udp::endpoint(boost::asio::ip::address_v4::any(), 3838);
    local_endpoint = udp::endpoint(boost::asio::ip::address_v4::any(), 3838);
    socket_.bind(local_endpoint);

    while (true)
    {
      uint8_t c_message[200];
      boost::system::error_code error;
      size_t m_length;

      m_length = socket_.receive_from(boost::asio::buffer(c_message), remote_endpoint, 0, error);

      char *message = (char *)c_message;

      pRoboCupGameControlData *converted_m = (pRoboCupGameControlData *)message;

      if(converted_m->version == GAMECONTROLLER_STRUCT_VERSION)
      {
        std::cout << "Primary State: ";
        switch (converted_m->state)
        {
          case STATE_INITIAL: std::cout << "Initial" << std::endl; break;
          case STATE_READY: std::cout << "Ready" << std::endl; break;
          case STATE_SET: std::cout << "Set" << std::endl; break;
          case STATE_PLAYING: std::cout << "Play" << std::endl; break;
          case STATE_FINISHED: std::cout << "Finish" << std::endl; break;
          default: std::cout << "None" << std::endl; break;
        }

        std::cout << "Secondary State: ";
        switch (converted_m->secondaryState)
        {
          case STATE2_NORMAL: std::cout << "Normal" << std::endl; break;
          case STATE2_PENALTYSHOOT: std::cout << "Penalty Shoot" << std::endl; break;
          case STATE2_OVERTIME: std::cout << "Overtime" << std::endl; break;
          case STATE2_TIMEOUT: std::cout << "Timeout" << std::endl; break;
          default: std::cout << "None" << std::endl; break;
        }

        std::cout << "Team Data:" << std::endl;
        
        switch (converted_m->teams[0].teamColour)
        {
          case TEAM_BLUE: std::cout << "  Blue Team Score: " << (int)converted_m->teams[0].score << std::endl; break;
          case TEAM_RED: std::cout << "  Red Team Score: " << (int)converted_m->teams[0].score << std::endl; break;
          default: std::cout << "None" << std::endl; break;
        }
        for (int i = 0; i < MAX_NUM_PLAYERS; i++)
        {
          switch (converted_m->teams[0].players[i].penalty)
          {
            case PENALTY_HL_TEEN_BALL_MANIPULATION: std::cout << "   Player " << i+1 << ": Ball Manipulation" << std::endl; break;
            case PENALTY_HL_TEEN_PHYSICAL_CONTACT: std::cout << "   Player " << i+1 << ": Physical Contact" << std::endl; break;
            case PENALTY_HL_TEEN_ILLEGAL_ATTACK: std::cout << "   Player " << i+1 << ": Illegal Attack" << std::endl; break;
            case PENALTY_HL_TEEN_ILLEGAL_DEFENSE: std::cout << "   Player " << i+1 << ": Illegal Defense" << std::endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_PICKUP: std::cout << "   Player " << i+1 << ": Request for Pickup" << std::endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_SERVICE: std::cout << "   Player " << i+1 << ": Request for Service" << std::endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_PICKUP_2_SERVICE: std::cout << "   Player " << i+1 << ": Request for Pickup from Service" << std::endl; break;
            default: break;
          }
        }

        switch (converted_m->teams[1].teamColour)
        {
          case TEAM_BLUE: std::cout << "  Blue Team Score: " << (int)converted_m->teams[1].score << std::endl; break;
          case TEAM_RED: std::cout << "  Red Team Score: " << (int)converted_m->teams[1].score << std::endl; break;
          default: std::cout << "None" << std::endl; break;
        }
        for (int i = 0; i < MAX_NUM_PLAYERS; i++)
        {
          switch (converted_m->teams[1].players[i].penalty)
          {
            case PENALTY_HL_TEEN_BALL_MANIPULATION: std::cout << "   Player " << i+1 << ": Ball Manipulation" << std::endl; break;
            case PENALTY_HL_TEEN_PHYSICAL_CONTACT: std::cout << "   Player " << i+1 << ": Physical Contact" << std::endl; break;
            case PENALTY_HL_TEEN_ILLEGAL_ATTACK: std::cout << "   Player " << i+1 << ": Illegal Attack" << std::endl; break;
            case PENALTY_HL_TEEN_ILLEGAL_DEFENSE: std::cout << "   Player " << i+1 << ": Illegal Defense" << std::endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_PICKUP: std::cout << "   Player " << i+1 << ": Request for Pickup" << std::endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_SERVICE: std::cout << "   Player " << i+1 << ": Request for Service" << std::endl; break;
            case PENALTY_HL_TEEN_REQUEST_FOR_PICKUP_2_SERVICE: std::cout << "   Player " << i+1 << ": Request for Pickup from Service" << std::endl; break;
            default: break;
          }
        }

        std::cout << std::endl;
      }

      if (error && error != boost::asio::error::message_size)
        throw boost::system::system_error(error);
      
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}