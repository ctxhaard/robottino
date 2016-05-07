#!/bin/bash

#set -e

source envsetup

#DRY_RUN=1

# enable pins
EN=(23 22)
# direction pins
DIR=(25 24)
# power pins
POW=(13 12)
GPIO="gpio -g"
ECHO=echo

if [ -n "$DRY_RUN" ]; then 
  GPIO="echo $GPIO" 
  ECHO=echo
fi

configure_pins() {
  echo --------- CONFIGURING PINS ---------
  for PIN in ${EN[*]}; do
    $GPIO mode $PIN output
    $GPIO write $PIN 1 
  done
  
  for PIN in ${DIR[*]}; do
    $GPIO mode $PIN output
    $GPIO write $PIN 1
  done
  
  for PIN in ${POW[*]}; do
    $GPIO mode $PIN pwm
    $GPIO pwm $PIN 0  
  done
}

stop() {
  $ECHO --------- STOP ---------
    for PIN in ${POW[*]}; do
      $GPIO pwm $PIN 0
    done  
}

# optional parameter set speed 0-10
forward() {
  $ECHO --------- FORWARD ---------
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 1
    done
    
    VAL=1023
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi
    for PIN in ${POW[*]}; do
      $GPIO pwm $PIN $VAL
    done
}

# optional parameter set speed 0-10
backward() {
  $ECHO --------- BACKWARD ---------
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 0
    done
    
    VAL=1023
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi
    for PIN in ${POW[*]}; do
      $GPIO pwm $PIN $VAL
    done
}

rotate_counter_clockwise() {
    $ECHO --------- ROTATE COUNTER-CLOKWISE ---------    
    $GPIO write ${DIR[0]} 0
    $GPIO write ${DIR[1]} 1
    
    for PIN in ${POW[*]}; do
      $GPIO pwm $PIN 1023
    done
}

rotate_clockwise() {
    $ECHO --------- ROTATE CLOKWISE ---------        
    $GPIO write ${DIR[0]} 1
    $GPIO write ${DIR[1]} 0
    
    for PIN in ${POW[*]}; do
      $GPIO pwm $PIN 1023
    done
}

# optional parameter set speed 0-10
forward_left() {
    $ECHO --------- FORWARD-LEFT ---------            
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 1
    done

    VAL=1023
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi

    $GPIO pwm ${POW[0]} $(( $VAL / 2 ))
    $GPIO pwm ${POW[1]} $VAL
}

# optional parameter set speed 0-10
forward_right() {
    $ECHO --------- FORWARD-RIGHT ---------                
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 1
    done

    VAL=1023
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi

    $GPIO pwm ${POW[0]} $VAL
    $GPIO pwm ${POW[1]} $(( $VAL / 2 ))
}

# optional parameter set speed 0-10
backward_left() {
    $ECHO --------- BACKWARD-LEFT ---------                    
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 0
    done

    VAL=1023
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi

    $GPIO pwm ${POW[0]} $(( $VAL / 2 ))
    $GPIO pwm ${POW[1]} $VAL
}

# optional parameter set speed 0-10
backward_right() {
    $ECHO --------- BACKWARD-RIGHT ---------                        
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 0
    done

    VAL=1023
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi

    $GPIO pwm ${POW[0]} $VAL
    $GPIO pwm ${POW[1]} $(( $VAL / 2 ))
}

sequence() {
echo AUTOMATIC SEQUENCE STARTED
  sleep 1
  forward 10
  sleep 3
  stop
  sleep 1
  backward 10
  sleep 3
  stop
  sleep 1
  rotate_clockwise
  sleep 2
  stop
  sleep 0.0
  rotate_counter_clockwise
  sleep 2
  stop
  sleep 1
  forward_left 10
  sleep 3
  stop
  sleep 1
  backward_right 10
  sleep 3
  stop
  sleep 1
  forward_right 10
  sleep 3
  stop
  sleep 1
  backward_left 10
  sleep 3
  stop
  echo AUTOMATIC SEQUENCE TERMINATED
}

start_video() {
  $ECHO --------- START-VIDEO ---------
  ./robottino-video &
  VIDEO_PID=$!
}

stop_video() {
  $ECHO --------- STOP-VIDEO ---------
  if [ -n "$VIDEO_PID" ]; then
    kill $VIDEO_PID
    unset VIDEO_PID
  fi
}

get_command() {
  while true; do
    read -t 0.5 -n 1 CMD
    $ECHO $CMD
    if [ -z "$CMD" ]; then
       CMD=5; 
    fi
    case $CMD in
      "6" )  rotate_clockwise;;
      "4" )  rotate_counter_clockwise;;
      "8" )  forward;;
      "2" )  backward;;
      "5" )  stop;;
      "9" )  forward_right;;
      "7" )  forward_left;;
      "1" )  backward_left;;
      "3" )  backward_right;;
      "/" )  start_video;;
      "*" )  stop_video;;
      * )  
        echo "cmd:??? $CMD"
        break
        ;;
    esac
  done
}

bash ./robottino-video &

configure_pins

while true; do
  echo listening on port $PORT
  nc -l $PORT | get_command
  echo terminated
done
