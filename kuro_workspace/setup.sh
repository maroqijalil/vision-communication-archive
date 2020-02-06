SH_FILE=$(readlink -f "$0")
SH_PATH=$(dirname "$SH_FILE")

export KURO_PATH=$SH_PATH

alias kuro='source ${KURO_PATH}/kuro.sh all'
alias kuro_cmake='source ${KURO_PATH}/kuro.sh kuro_cmake'
alias kuro_dynamixel='source ${KURO_PATH}/kuro.sh kuro_dynamixel'
alias kuro_head='source ${KURO_PATH}/kuro.sh kuro_head'
alias kuro_interface='source ${KURO_PATH}/kuro.sh kuro_interface'
alias kuro_motion='source ${KURO_PATH}/kuro.sh kuro_motion'
alias kuro_robot='source ${KURO_PATH}/kuro.sh kuro_robot'
alias kuro_simulation='source ${KURO_PATH}/kuro.sh kuro_simulation'
alias kuro_utility='source ${KURO_PATH}/kuro.sh kuro_utility'
alias kuro_vision='source ${KURO_PATH}/kuro.sh kuro_vision'
alias kuro_walk='source ${KURO_PATH}/kuro.sh kuro_walk'

if [[ $SHELL == *"zsh"* ]]
then
  LOCAL_SETUP_SH_FILE=${KURO_PATH}'/install/local_setup.zsh'
  ROS_SETUP_SH_FILE='/opt/ros/dashing/setup.zsh'
else
  LOCAL_SETUP_SH_FILE=${KURO_PATH}'/install/local_setup.sh'
  ROS_SETUP_SH_FILE='/opt/ros/dashing/setup.sh'
fi

if [ -f $ROS_SETUP_SH_FILE ]
then
  source $ROS_SETUP_SH_FILE
else
  echo '\033[31mError: '$ROS_SETUP_SH_FILE' file not found\033[0m'
  return 1
fi

if [ -f $LOCAL_SETUP_SH_FILE ]
then
  source $LOCAL_SETUP_SH_FILE
else
  echo '\033[31mError: '$LOCAL_SETUP_SH_FILE' file not found\033[0m'
  return 1
fi

export RMW_IMPLEMENTATION=rmw_opensplice_cpp

return 0
