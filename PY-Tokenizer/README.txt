This project is a text tokenizer that aims to help improve search engine efficiency beyond just basic exact query match. It has three options and produces 3 files.
The first option is the type of tokenizer, either "spaces" or "fancy". The spaces option just tokenizes the input by spaces, and the fancy goes through a detailed ruleset
that improves the tokenizing.
The second option is stopping or not. If stopping is selected, all tokens from a set of words (a, and, or..) that do not contribute much to a query or document are removed.
The third option is a modified implementation of the Porter Stemmer, an algorithm that transforms words into their roots so that many words that mean nearly identical things
are combined into one.
This program takes in a compressed gzip file, and outputs 3 different files:
tokens: This file has all the tokens after they have been processed in the form "tokens processedTokens" where tokens is simply the words split by spaces and processedTokens is
the tokens after whatever processing is done to it.
heaps: This file will have a line for every 10 tokens created by the tokenizer as well as the number of unique tokens thus far, in form "totalTokens uniqueTokens".
stats: this file will have the total number of tokens, followed by total number of unique tokens, followed by each word and how many times it occured in a document.
In order to run the program, you simply run it by command line argument "python tokens.py (inputfile) (outputPrefix) (spaces/fancy) (noStop/stop) (nostem/stem)" selecting the options promptly.
The inputfile must be in gzip compressed format for this to work, and the outputPrefix will be the prefix to the three created output files.
Provided is 3 output files run on a long section of a book called "Senses and Sensibility", although you are free to run your own gzip with different settings and see how it works.
