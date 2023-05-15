import { fetchCurrentWeather } from "./fetchCurrentWeather.js"
import { fetchLongitudeAndLatitude } from "./fetchLongitudeAndLatitude.js"
import { fetchUniversities } from "./fetchUniversities.js"
// Here is a function that fetches the weather at a specific university given a query.
export function fetchUniversityWeather(query) {
  // First we call upon fetchUniversities in order to find the list of universities, 
  return fetchUniversities(query) 
        .then(universities => {
          if(universities.length === 0){return Promise.reject(new Error("No results found for query."))}
          let ans = new Object()
        // then for each university, we asynchronously fetch the longitude and latitude for each one, then use that information to call fetch Current Weather. We finally find the average temperature and return it.
          universities.forEach(
            async p => ans[p] = await fetchLongitudeAndLatitude(p)
                                                .then(obj => fetchCurrentWeather(obj.lon, obj.lat))
                                                .then(obj => obj.temperature_2m)
                                                .then(array => array.reduce((acc, e) => acc+e, 0)/array.length))
          ans.totalAverage = Object.values(ans).reduce((acc,e) => acc+e, 0)/universities.length;
          return ans;
          })
}

// This function fetches the weather at umass. It just calls upon the above function with query University of Massachusetts.
export function fetchUMassWeather() {
  return fetchUniversityWeather("University of Massachusetts");
}
//This function does the same as above, it just fetches the weather at ucal by calling the above function with the query University of California.
export function fetchUCalWeather() {
  return fetchUniversityWeather("University of California");
}
