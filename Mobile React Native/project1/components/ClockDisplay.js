import React from "react"
import { StyleSheet, Text } from "react-native"
import PropTypes from "prop-types"

const ClockDisplay = props => {
  const clockify = () => {
    const minutes = Math.floor(props.timer / 60),
          seconds = props.timer % 60
    if (minutes < 10 && seconds >= 10) {
      return "0" + minutes + ":" + seconds
    } else if (minutes < 10 && seconds < 10) {
      return "0" + minutes + ":" + "0" + seconds
    } else  if (minutes >= 10 && seconds < 10) {
      return minutes + ":" + "0" + seconds
    } else {
      return minutes + ":" + seconds
    }
  }

  return (
    <Text style={styles.timeLeft}>{clockify()}</Text>
  )
}

ClockDisplay.propTypes = {
  timer: PropTypes.number.isRequired,
  running: PropTypes.bool.isRequired,
  workTime: PropTypes.number.isRequired,
  breakTime: PropTypes.number.isRequired,
  status: PropTypes.string.isRequired
}

const styles = StyleSheet.create({
  timeLeft: {
    fontSize: 30
  },
})

export default ClockDisplay