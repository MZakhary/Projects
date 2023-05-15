import { readFile, writeFile } from "node:fs/promises";
// These are simple file utility functions that write and read from JSON files. 
export function writeToJSONFile(path, data) {
    return writeFile(path, JSON.stringify(data));
  }

export function readFromJSONFile(path) {
  return readFile(path).then(x=>JSON.parse(x));
}
