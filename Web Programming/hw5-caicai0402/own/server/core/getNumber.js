let number

const getNumber = (forceRestart = false) => {
  if(forceRestart == true){
    number = Math.floor(Math.random() * 100) + 1
    console.log("number=", number)
  }
  // TODO:
  // generate a random number if number is undefined or forceRestart is true
  return number
}

export default getNumber
