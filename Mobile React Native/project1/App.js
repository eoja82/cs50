/* https://snack.expo.io/mIG7TqB4I */
/* https://snack.expo.io/R1ST9Ct4U */
import { StatusBar } from 'expo-status-bar'
import React, { useState, useEffect } from 'react'
import { StyleSheet, Text, Vibration, View } from 'react-native'

import TimerControl, { PlayPauseReset } from "./components/TimeControl"
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
    //console.log(intervalId)
    clearInterval(intervalId)
  }
  const breakUp = () => {
    if (breakTime < 3600) setBreakTime(prevBreakTime => {
      if (status === "Break" && !running) setTimer(prevBreakTime + 60)
      return prevBreakTime + 60
    })
  }
  const breakDown = () => {
    if (breakTime > 60) setBreakTime(prevBreakTime => {
      if (status === "Break" && !running) setTimer(prevBreakTime - 60)
      return prevBreakTime - 60
    })
  }
  const workUp = () => {
    if (workTime < 3600) setWorkTime(prevWorkTime => {
      if (status === "Work" && !running) setTimer(prevWorkTime + 60)
      return prevWorkTime + 60
    })
  }
  const workDown = () => {
    //console.log("workTime: " + workTime)
    if (workTime > 60) {
      setWorkTime(prevWorkTime => {
        if (status === "Work" && !running) setTimer(prevWorkTime - 60)
        return prevWorkTime - 60
      })
    }
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
  const timerAtZero = (newStatus, newTimer) => {
    console.log("vibrate")
    Vibration.vibrate([500, 500, 500])
    setStatus(newStatus)
    setTimer(newTimer)
    //clearInterval(intervalId)
  }
  useEffect(() => {
    if (status === "Work" && timer < 0) {
      timerAtZero("Break", breakTime)
    }
    if (status === "Break" && timer < 0) {
      timerAtZero("Work", workTime)
    }
  })

  return (
    <View style={styles.container}>
      <Text style={styles.heading}>Pomodoro Clock</Text>
      <TimerControl 
        text={"Set Break Time: "} 
        time={breakTime / 60} 
        funcOne={breakUp} 
        funcTwo={breakDown}
      />
      <TimerControl 
        text={"Set Work Time: "}
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
      <ClockDisplay 
        timer={timer} 
        running={running}
        workTime={workTime}
        breakTime={breakTime}
        status={status}
      />
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
