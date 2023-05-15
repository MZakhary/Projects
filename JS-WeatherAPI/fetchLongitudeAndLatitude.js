// This function finds the longitude and latitude of a specific query using an API. 
export function fetchLongitudeAndLatitude(query) {
  // Creating the query and sending to the API
  const url = new URL("https://geocode-cache.herokuapp.com/search");
  url.searchParams.append("q", query);
  // Returns a promise that fullfills if the returned data is found with the longitude and latitude, rejecting otheriwse.
  return fetch(url.toString())
  .then(response => response.json())
  .then((data) => {
    if(Array.isArray(data)) {
      return data.length === 0
      ? Promise.reject(new Error("No results found for query"))
      : Promise.resolve({lon: Number(data[0].lon), lat: Number(data[0].lat)});
    }
  }
  )
}