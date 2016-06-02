#!/bin/bash

set -e

source envsetup

#export DRY_RUN=1
export VERBOSE=1

# enable pins
export EN=(23 22)
# direction pins
export DIR=(25 24)
# power pins
export POW=(13 12)
export GPIO="gpio -g"
export ECHO=":"
export PWM_DEF_RO="550"

export CORNER_RATIO_RO="0" 
#export CORNER_RATIO_RO="9 / 10" 
export CORNER_RATIO="$CORNER_RATIO_RO" 

if [ -n "$DRY_RUN" ]; then 
  GPIO="echo $GPIO" 
fi

if [ -n "$VERBOSE" ]; then
  ECHO="echo"  
fi

if [ -n "$1" ]; then
    PWM_DEF_RO="$1"
fi
export PWM_DEF=$PWM_DEF_RO
echo "pwm power set to: $PWM_DEF"

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
forward() (
    $ECHO --------- FORWARD ---------
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 1
    done
    
    VAL="$PWM_DEF"
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi
    for PIN in ${POW[*]}; do
      $GPIO pwm $PIN $VAL
    done
)

# optional parameter set speed 0-10
backward() {
  $ECHO --------- BACKWARD ---------
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 0
    done
    
    VAL="$PWM_DEF"
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
      $GPIO pwm $PIN "$PWM_DEF"
    done
}

rotate_clockwise() {
    $ECHO --------- ROTATE CLOKWISE ---------        
    $GPIO write ${DIR[0]} 1
    $GPIO write ${DIR[1]} 0
    
    for PIN in ${POW[*]}; do
      $GPIO pwm $PIN "$PWM_DEF"
    done
}

# optional parameter set speed 0-10
forward_left() {
    $ECHO --------- FORWARD-LEFT ---------            
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 1
    done

    VAL="$PWM_DEF"
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi
    $GPIO pwm ${POW[0]} $(( $VAL * $CORNER_RATIO ))
    $GPIO pwm ${POW[1]} $VAL
}

# optional parameter set speed 0-10
forward_right() {
    $ECHO --------- FORWARD-RIGHT ---------                
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 1
    done

    VAL="$PWM_DEF"
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi
    $GPIO pwm ${POW[0]} $VAL
    $GPIO pwm ${POW[1]} $(( $VAL * $CORNER_RATIO ))
}

# optional parameter set speed 0-10
backward_left() {
    $ECHO --------- BACKWARD-LEFT ---------                    
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 0
    done

    VAL="$PWM_DEF"
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi

    $GPIO pwm ${POW[0]} $(( $VAL * $CORNER_RATIO ))
    $GPIO pwm ${POW[1]} $VAL
}

# optional parameter set speed 0-10
backward_right() {
    $ECHO --------- BACKWARD-RIGHT ---------                        
    for PIN in ${DIR[*]}; do
      $GPIO write $PIN 0
    done

    VAL="$PWM_DEF"
    if [ -n "$1" ]; then
      VAL=$(($1 * 1023 / 10))
    fi

    $GPIO pwm ${POW[0]} $VAL
    $GPIO pwm ${POW[1]} $(( $VAL * $CORNER_RATIO ))
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

power_normal() {
  $ECHO --------- POWER-NORMAL ---------
  PWM_DEF=$PWM_DEF_RO
  CORNER_RATIO=$CORNER_RATIO_RO
}

power_max() {
  $ECHO ---------- POWER-MAX ----------
  PWM_DEF=1023
  CORNER_RATIO="1 / 2"
}

get_command() {
  while true; do
    read -t 2 -n 1 CMD || true
    if [ -z "$CMD" ]; then
       CMD=5; 
    fi
    $ECHO $CMD
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
      "+" )  power_max;;
      "-" )  power_normal;;
      * )  
        echo "cmd:??? $CMD"
        break
        ;;
    esac
  done
}

#bash ./robottino-video &

configure_pins

while true; do
  echo listening on port $PORT
  nc -k -l $PORT | get_command
  echo terminated
done

