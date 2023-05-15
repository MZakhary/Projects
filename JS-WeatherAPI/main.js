
import { writeToJSONFile, readFromJSONFile } from "./fileUtility.js"
import { fetchCurrentWeather } from "./fetchCurrentWeather.js"
import { readFile, writeFile } from "node:fs/promises";
//This is the main function of the program, that finds a random latitude and longitude and then uses the fetch current weather asynchronously in order to provide temperature information 
// about said coordinate.
async function main() {
let randLat = Math.random()*89;
let randLong = Math.random()*89;
let dat = "Here's a fun fact. The weather at coordinates:" + randLong + " and " + randLat + " on the day at midnight";
let weather = "";
await fetchCurrentWeather(randLong, randLat).then(x=>weather=x.time[0].substring(0,10) + " was " + x.temperature_2m[0])
let data = dat.concat(weather);
    try {
        await writeFile("main.json", data);
            readFile("./main.json").then(x=>console.log(x.toString()));
    } catch (err) {
        return err;
    }
};
main();