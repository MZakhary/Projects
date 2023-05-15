// This function uses the api provided in order to search and find the current weather of a specific latitude and longitude coordinate.
export function fetchCurrentWeather(longitude, latitude) {
  // Creating the search parameters based on function input.
  let u = new URL('https://api.open-meteo.com/v1/forecast');
  u.searchParams.append("latitude", latitude);
  u.searchParams.append("longitude", longitude);
  u.searchParams.append("hourly", "temperature_2m");
  u.searchParams.append("temperature_unit", "fahrenheit");
  // I then use fetch while using a promise in order to await a response and then if the promise resolves successfully, I return the time and temperature of said longitude/latitude
  // If the promise reject, it just returns "failed".
  return fetch(u.toString()).then(response => response.json())
  .then((d) => {
    if(Array.isArray(d.hourly.time) && Array.isArray(d.hourly.temperature_2m))
    {
    let t = d.hourly.time.map(x=>x);
    let t_2m= d.hourly.temperature_2m.map(x=>x);
    if(t.length!==0 && t_2m.length !==0)
    return Promise.resolve({time:t, temperature_2m:t_2m});
    }
    return Promise.reject("failed");
  })
}
