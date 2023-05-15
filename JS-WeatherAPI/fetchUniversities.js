// This function fetches the universities off an api given a query.
export function fetchUniversities(query) {
  // Constructing the query
  const url = new URL("https://university-web-api.herokuapp.com/search");
  url.searchParams.append("name", query);
  // Then we return a promise that if successful, provides the names of the universities.
  return fetch(url.toString())
  .then(data => data.json())
  .then(data => {
      return data.map(x => x.name)
    }
  )
}
