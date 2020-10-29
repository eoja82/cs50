/* https://snack.expo.io/mIG7TqB4I */
/* https://snack.expo.io/R1ST9Ct4U */
import { StatusBar } from 'expo-status-bar'
import React, { useState, useEffect } from 'react'
import { StyleSheet, Text, TouchableOpacity, Vibration, View } from 'react-native'
import { AntDesign } from '@expo/vector-icons'
import { FontAwesome } from '@expo/vector-icons'

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
  const playPauseIcon = () => {
    if (!running) {
      return ( <FontAwesome name="play" style={styles.icons} /> )
    } else {
      return ( <FontAwesome name="pause" style={styles.icons} /> )
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
    intervalId = setInterval(consolidateFunc, 1000)
  }
  const consolidateFunc = () => {
    countDown()
    //buzz()
  }
  const countDown = () => {
    //console.log("coutdown timer " + timer)
    setTimer(timer => timer - 1)
  }
  /* const buzz = () => {
    //console.log("timer in buzz " + timer)
    if (status === "Work" && timer < 0) {
      // buzz
      setStatus("Break")
      setTimer(breakTime)
    }
    if (status === "Break" && timer < 0) {
      // buzz
      setStatus("Work")
      setTimer(workTime)
    }
  } */
  const clockify = () => {
    //console.log("timer in clock " + timer)
    const minutes = Math.floor(timer / 60),
          seconds = timer % 60
    if (seconds < 10 && minutes < 10) {
      return "0" + minutes + ":" + "0" + seconds;
    } else if (seconds < 10) {
      return minutes + ":" + "0" + seconds;
    } else {
      return minutes + ":" + seconds;
    }
  }
  useEffect(() => {
    console.log("timer: " + timer, "status: " + status)
    if (status === "Work" && timer < 0) {
      console.log("vibrate")
      Vibration.vibrate([500, 500, 500])
      setStatus("Break")
      setTimer(breakTime)
      //clearInterval(intervalId)
    }
    if (status === "Break" && timer < 0) {
      console.log("vibrate")
      Vibration.vibrate([500, 500, 500])
      setStatus("Work")
      setTimer(workTime)
    }
  })

  return (
    <View style={styles.container}>
      <Text style={styles.heading}>Pomodoro Clock</Text>
      <View style={styles.controls}>
        <Text style={styles.controlText}>Set Break Time </Text>
        <Text style={styles.controlText}>{breakTime / 60}</Text>
        <TouchableOpacity onPress={breakUp}><AntDesign name="upcircleo" style={styles.icons} /></TouchableOpacity>
        <TouchableOpacity onPress={breakDown}><AntDesign name="downcircleo" style={styles.icons} /></TouchableOpacity>
      </View>
      <View style={styles.controls}>
        <Text style={styles.controlText}>Set Work Time </Text>
        <Text style={styles.controlText}>{workTime / 60}</Text>
        <TouchableOpacity onPress={workUp}><AntDesign name="upcircleo" style={styles.icons} /></TouchableOpacity>
        <TouchableOpacity onPress={workDown}><AntDesign name="downcircleo" style={styles.icons} /></TouchableOpacity>
      </View>
      <View style={styles.controls}>
        <TouchableOpacity onPress={playPause} style={styles.reset}>{playPauseIcon()}</TouchableOpacity>
        <TouchableOpacity style={styles.reset} onPress={reset}>
          <FontAwesome name="undo" style={styles.icons} />
        </TouchableOpacity>
      </View>
      <Text style={styles.status}>{`${status} Time Remaining`}</Text>
      <Text style={styles.timeLeft}>{clockify()}</Text>
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
  timeLeft: {
    fontSize: 30
  },
  controls: {
    flexDirection: "row",
    alignItems: "center"
  },
  controlText: {
    fontSize: 30
  },
  icons: {
    fontSize: 25,
    color: "black",
    paddingLeft: 10
  },
  reset: {
    paddingTop: 15,
    paddingRight: 5,
    paddingLeft: 5
  }
});
