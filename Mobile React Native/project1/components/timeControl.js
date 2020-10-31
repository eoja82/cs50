import React from "react"
import { StyleSheet, View, Text, TouchableOpacity } from "react-native"
import PropTypes from "prop-types"
import { AntDesign } from '@expo/vector-icons'
import { FontAwesome } from '@expo/vector-icons'

const TimerControl = props => (
  <View style={styles.controls}>
    <Text style={styles.controlText}>{`${props.text} `}</Text>
    <Text style={styles.controlText}>{props.time}</Text>
    <TouchableOpacity onPress={props.funcOne}><AntDesign name="arrowup" style={styles.icons} /></TouchableOpacity>
    <TouchableOpacity onPress={props.funcTwo}><AntDesign name="arrowdown" style={styles.icons} /></TouchableOpacity>
  </View>
)

TimerControl.propTypes = {
  text: PropTypes.string.isRequired,
  time: PropTypes.number.isRequired,
  funcOne: PropTypes.func.isRequired,
  funcTwo: PropTypes.func.isRequired
}

export const PlayPauseReset = props => {
  const playPauseIcon = () => {
    if (!props.running) {
      return ( <FontAwesome name="play" style={styles.icons} /> )
    } else {
      return ( <FontAwesome name="pause" style={styles.icons} /> )
    }
  }
  return (
    <View style={styles.controls}>
      <TouchableOpacity onPress={props.playPause} style={styles.reset}>{playPauseIcon()}</TouchableOpacity>
      <TouchableOpacity style={styles.reset} onPress={props.reset}>
        <FontAwesome name="undo" style={styles.icons} />
      </TouchableOpacity>
    </View>
  )
}

PlayPauseReset.propTypes = {
  running: PropTypes.bool.isRequired,
  playPause: PropTypes.func.isRequired,
  reset: PropTypes.func.isRequired
}

const styles = StyleSheet.create({
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
})

export default TimerControl