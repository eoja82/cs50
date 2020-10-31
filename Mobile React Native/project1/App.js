/* https://snack.expo.io/mIG7TqB4I */
/* https://snack.expo.io/R1ST9Ct4U */
import { StatusBar } from 'expo-status-bar'
import React, { useState, useEffect } from 'react'
import { StyleSheet, Text, Vibration, View } from 'react-native'

import TimerControl, { PlayPauseReset } from "./components/timeControl"
import ClockDisplay from "./components/ClockDisplay"

let intervalId = null

export default function App() {
  const [workTime, setWorkTime] = useState(1500),
        [breakTime, setBreakTime] = useState(300),
        [timer, setTimer] = useState(1500),
        [status, setStatus] = useState("Work"),
        [running, setRunning] = useState(false)
  
  const reset = () => {
    setWorkTime(1500)
    setBreakTime(300)
    setTimer(1500)
    setStatus("Work")
    setRunning(false)
    console.log(intervalId)
    clearInterval(intervalId)
  }
  const breakUp = () => {
    if (breakTime < 3600) setBreakTime(breakTime + 60)
  }
  const breakDown = () => {
    if (breakTime > 60) setBreakTime(breakTime - 60)
  }
  const workUp = () => {
    if (workTime < 3600) setWorkTime(workTime + 60)
  }
  const workDown = () => {
    if (workTime > 60) setWorkTime(workTime - 60)
  }
  const playPause = () => {
    if (!running) {
      setRunning(true)
      run()
    } else {
      setRunning(false)
      clearInterval(intervalId)
    }
  }
  const run = () => {
    intervalId = setInterval(countDown, 1000)
  }
  const countDown = () => {
    //console.log("coutdown timer " + timer)
    setTimer(timer => timer - 1)
  }
  useEffect(() => {
    console.log("timer: " + timer, "status: " + status)
    if (status === "Work" && timer < 1497) {
      console.log("vibrate")
      Vibration.vibrate([500, 500, 500])
      setStatus("Break")
      setTimer(breakTime)
      //clearInterval(intervalId)
    }
    if (status === "Break" && timer < 297) {
      console.log("vibrate")
      Vibration.vibrate([500, 500, 500])
      setStatus("Work")
      setTimer(workTime)
    }
  })

  return (
    <View style={styles.container}>
      <Text style={styles.heading}>Pomodoro Clock</Text>
      <TimerControl 
        text={"Set Break Time"} 
        time={breakTime / 60} 
        funcOne={breakUp} 
        funcTwo={breakDown}
      />
      <TimerControl 
        text={"Set Work Time"}
        time={workTime / 60}
        funcOne={workUp}
        funcTwo={workDown}
      />
      <PlayPauseReset 
        running={running} 
        playPause={playPause}
        reset={reset}
      />
      <Text style={styles.status}>{`${status} Time Remaining`}</Text>
      <ClockDisplay timer={timer} />
      <StatusBar style="auto" />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  heading: {
    fontSize: 50,
    paddingBottom: 20
  },
  status: {
    fontSize: 30,
    paddingTop: 20
  },
});
