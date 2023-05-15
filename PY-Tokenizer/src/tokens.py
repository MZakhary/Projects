import sys
import gzip
import re

if __name__ == '__main__':
    # Read arguments, and using default.
    argv_len = len(sys.argv)
    inputFile = sys.argv[1] if argv_len >= 2 else "input.gz"
    outputFilePrefix = sys.argv[2] if argv_len >= 3 else "output"
    tokenize_type = sys.argv[3] if argv_len >= 4 else "fancy"
    stoplist_type = sys.argv[4] if argv_len >= 5 else "stop"
    stemming_type = sys.argv[5] if argv_len >= 6 else "stem"

    # Below is stopword list, Words here are removed from the input text if the stopping is selected.
    stopword_lst = stopword_lst = {"a", "an", "and", "are", "as", "at", "be", "by", "for", "from",
                    "has", "he", "in", "is", "it", "its", "of", "on", "that", "the", "to",
                    "was", "were", "with"}
# Here are two regular expressions, one is for URLS and one is for numbers. This allows detection of numbers or URLS in order to leave them in their purest form for search purposes.
url_regex = re.compile(r'(https?://\S+)[^\w\s]*')
number_regex = re.compile(r"(?<![A-Za-z])[\+\-\d,\.]+(?![A-Za-z])")
# Opening the input and output files to begin processing the input and writing to output file.
with gzip.open(sys.argv[1], 'rt') as f:
    with open(sys.argv[2]+"-tokens.txt", "w") as out:
        # For every line in the input file, we split the line into tokens by space. Then depending on which tokenization option is selected, the tokens are written in the output file.
        # For just the basic "spaces", tokenizing is only done by spaces (meaning no further processing.) This is simple but inefficient when it comes to search purposes.
        # For the "fancy" option, tokenizing has rules specified below that aid in tokenizing. 
        # Note that in the output file, each token is listed as tokenized by spaces before being listed as tokenized as the selected option.
        # This means the token will be repeated twice for the "spaces" option, but will be different for the "fancy" option for ease of comparison of the tokenizer.
        for line in f:
            tokens = line.split()
            if sys.argv[3] == "spaces":
                for token in tokens:
                    # Leave tokens as is, tokenized by spaces only.
                    out.write(token + ' ')
                    out.write(token + '\n')
            if sys.argv[3] == "fancy":
                for token in tokens:
                    out.write(token + " ")
                    # First we check if the token matches a URL, if it does, then we keep the token as is and only remove any leading slashes.
                    if url_regex.match(token):
                        while token and not token[-1].isalnum() and token[-1] != '/':
                            token = token[:-1]
                        out.write(token + '\n')
                        continue            
                    # All tokens are converted to lowercase for ease of search (other than URLS) 
                    token = token.lower()
                    # Number tokens have all their punctuation stripped, then are written to the output file.
                    if number_regex.match(token):
                        token = token.replace('/', '')
                        token = token.replace('!', '')
                        token = token.replace('@', '')
                        token = token.replace('#', '')
                        token = token.replace('$', '')
                        token = token.replace('%', '')
                        token = token.replace('^', '')
                        token = token.replace('&', '')
                        token = token.replace('*', '')
                        token = token.replace('(', '')
                        token = token.replace(')', '')
                        token = token.replace('=', '')
                        out.write(token + '\n')
                        continue
                    # Apostrophes are replaced in tokens
                    token = token.replace("'", "")
                    # All other punctuation aside from periods and hyphens are used as tokenizing stops (Aka phrases with any of the symbols in delims are treated as seperate words)
                    delim = "/!@#$%^&*()=+_,\;:\" "
                    lasttokens = [lasttok for lasttok in re.split('[' + re.escape(delim) + ']', token) if lasttok]
                    # This applies rules for all the words created by the tokenizing stops in delim, removing any hyphens and periods. Tokens like "p.h.d" or other abbreviations have their periods stripped.
                    # Hyphenated words are tokenized as the word without hyphens, then each word individually (Ex: data-base becomes the tokens data, base, and database.)
                    for i,tok in enumerate(lasttokens):
                        if '-' in tok:
                            hyphentok = tok.split('-')
                            for toke in hyphentok:
                                toke = toke.replace(".", "")
                                out.write(toke + " ")
                            out.write(tok.replace('-','').replace('.','') + "\n")
                            continue
                        elif tok.count('.') > 0:
                            tok = tok.replace(".", "")
                            out.write(tok + '\n')
                            continue
                        if i+1 == len(lasttokens):
                            out.write(tok + '\n')
                        else:
                            out.write(tok+" ")
# If the stopping option is selected, the file then goes through a stopping list in which all words in the above stopping list are removed. This is done in order to reduce
# clutter and words that do not add much value to the information in the file.
if sys.argv[4]=="stop":
    # Reading the output file and saving to memory
    with open(sys.argv[2]+"-tokens.txt", 'r') as f:
        lines = f.readlines()
    # Reopening the output file, then for each line in the output file, it strips it back into tokens and removes the stop words in the above list.
    with open(sys.argv[2]+"-tokens.txt", 'w') as f:
        for line in lines:
            # Remove stop words from the line and write it back to the file
            words = line.strip().split()
            filtered_words = [words[0]] + [w for w in words[1:] if w not in stopword_lst]
            filtered_line = ' '.join(filtered_words) + '\n'
            f.write(filtered_line)

# If the stemming option is selected, a modified version of the Porter Stemmer is applied to the list. This porter stemmer breaks words that have similar meanings (ran/run/running, ate/eat, ect)
# Note that this porter stemmer option when selected can and will cause some words to become non words. This is not a bug, it is a part of the porter stemmer algorithm.
if sys.argv[5]=="stem":
    # Opening the output file and saving to memory
    with open(sys.argv[2]+"-tokens.txt", 'r') as f:
        lines = f.readlines()
    # Opening the output file to edit.
    with open(sys.argv[2]+"-tokens.txt", 'w') as f:
        for line in lines:
            # Tokenize the line
            tokens = line.strip().split()
            # Stem each token except the first one on each line (since the first word in the output should be the base token.)
            for i in range(1, len(tokens)):
                token = tokens[i]
                stem = token
                # The following algorithm follows 3 steps. For more information, visit https://en.wikipedia.org/wiki/Stemming
                # Note that this is a modified version of the porter stemmer. The tokens go through steps 1-3, with each step attempting different cases depending on the token.
                # If the token matches some part of any step, it is modified and immediately goes on to the next step.
                # Step 1
                # Handling words that end with sses, ies, us, or s.
                if token.endswith('sses'):
                    stem = token[:-2]
                elif token.endswith('ies') or token.endswith('ied'):
                    if len(token) > 4:
                        stem = token[:-2]
                    else:
                        stem = token[:-1]
                elif token.endswith('us') or token.endswith('ss'):
                    pass
                elif token.endswith('s'):
                    if any(c in 'aeiouy' for c in token[:-2]):
                        stem = token[:-1]

                # Step 2
                # Handling words that end with eedly, ed, edly, or ing. Note that in each one, it also handles words that end with at/bl/iz, and special extra small words.
                if stem.endswith('eedly') or stem.endswith('eed'):
                    if True:
                        if re.search(re.compile(r'[aeiouy][^aeiouy]*eed'),stem) is not None:
                            stem = stem[:-1]
                        if re.search(re.compile(r'[aeiouy][^aeiouy]*eedly'),stem) is not None:
                            stem = stem[:-3] 
                elif stem.endswith('ed'):
                    if not any(c in 'aeiouy' for c in stem[:-2]):
                        pass
                    else:
                        stem = stem[:-2]
                        if stem.endswith(('at', 'bl', 'iz')):
                            stem += 'e'
                            pass
                        elif stem.endswith('bb') or stem.endswith('dd') or stem.endswith('ff') or stem.endswith('gg') or stem.endswith('mm') or stem.endswith('nn') or stem.endswith('pp') or stem.endswith('rr') or stem.endswith('tt'):
                            stem = stem[:-1]
                            pass
                        elif len(stem) == 2 and stem[-2] in 'aeiouy' and not stem[-1] in 'aeiouy':
                            stem += 'e'
                        elif len(stem)>=3:
                            if stem[-1] not in 'wxaeiouy' and stem[-2] in 'aeiouy' and not any(c in 'aeiouy' for c in stem[:-3]):
                                stem += 'e'
                elif stem.endswith('edly'):
                    if not any(c in 'aeiouy' for c in stem[:-4]):
                        pass
                    else:
                        stem = stem[:-4]
                        if stem.endswith(('at', 'bl', 'iz')):
                            stem += 'e'
                            pass
                        elif stem.endswith('bb') or stem.endswith('dd') or stem.endswith('ff') or stem.endswith('gg') or stem.endswith('mm') or stem.endswith('nn') or stem.endswith('pp') or stem.endswith('rr') or stem.endswith('tt'):
                            stem = stem[:-1]
                            pass
                        elif len(stem) == 2 and stem[-2] in 'aeiouy' and not stem[-1] in 'aeiouy':
                            stem += 'e'
                        elif len(stem)>=3:
                            if stem[-1] not in 'wxaeiouy' and stem[-2] in 'aeiouy' and not any(c in 'aeiouy' for c in stem[:-3]):
                                stem += 'e'
                elif stem.endswith('ing'):
                    if not any(c in 'aeiouy' for c in stem[:-3]):
                        pass
                    else:
                        stem = stem[:-3]
                        if stem.endswith(('at', 'bl', 'iz')):
                            stem += 'e'
                            pass
                        elif stem.endswith('bb') or stem.endswith('dd') or stem.endswith('ff') or stem.endswith('gg') or stem.endswith('mm') or stem.endswith('nn') or stem.endswith('pp') or stem.endswith('rr') or stem.endswith('tt'):
                            stem = stem[:-1]
                            pass
                        elif len(stem) == 2 and stem[-2] in 'aeiouy' and not stem[-1] in 'aeiouy':
                            stem += 'e'
                        elif len(stem)>=3:
                            if stem[-1] not in 'wxaeiouy' and stem[-2] in 'aeiouy' and not any(c in 'aeiouy' for c in stem[:-3]):
                                stem += 'e'
                elif stem.endswith('ingly'):
                    if not any(c in 'aeiouy' for c in stem[:-4]):
                        pass
                    else:
                        stem = stem[:-5]
                        if stem.endswith(('at', 'bl', 'iz')):
                            stem += 'e'
                            pass
                        elif stem.endswith('bb') or stem.endswith('dd') or stem.endswith('ff') or stem.endswith('gg') or stem.endswith('mm') or stem.endswith('nn') or stem.endswith('pp') or stem.endswith('rr') or stem.endswith('tt'):
                            stem = stem[:-1]
                            pass
                        elif len(stem) == 2 and stem[-2] in 'aeiouy' and not stem[-1] in 'aeiouy':
                            stem += 'e'
                        elif len(stem)>=3:
                            if stem[-1] not in 'wxaeiouy' and stem[-2] in 'aeiouy' and not any(c in 'aeiouy' for c in stem[:-3]):
                                stem += 'e'
                # Step 3
                # Finally, handling words that end with y.
                if stem.endswith('y'):
                    if len(token) > 2:
                        if stem[-2] not in 'aeiouy':
                            stem = stem[:-1] + 'i'

                tokens[i] = stem

            # Join the stemmed tokens and write back to the file
            stemmed_line = ' '.join(tokens) + '\n'
            f.write(stemmed_line)




# Creating the Heap file. The heap file holds information regarding the number of processed tokens versus the number of unique tokens for every 10 tokens. 
# This takes the shape of "totalTokens UniqueTokens" in the generated "heaps" file. 
# Opens the output tokens file and creates a heaps file.
with open(sys.argv[2]+"-tokens.txt", "r") as f:
    with open(sys.argv[2]+"-heaps.txt", "w") as out:
        total_tokens = 0
        unique_tokens = set()
        # For every line in the tokens, it simply goes through all the tokens and every 10 tokens increments the total number of tokens by 10, then checks every token against a set
        # of tokens to increment unique tokens as well. For every 10 tokens, a new line is written in the heaps file.
        for line in f:
            tokens = line.split()[1:]
            total_tokens += len(tokens)
            for token in tokens:
                if token not in unique_tokens:
                    unique_tokens.add(token)
            if total_tokens % 10 == 0:
                if tokens:
                    out.write(f"{total_tokens} {len(unique_tokens)}\n")
        out.write(f"{total_tokens} {len(unique_tokens)}\n")


# Creating the stats file. The stats file holds information about how many unique times each term occurs. The first two lines in the file is the total number of tokens followed by the
# total number of unique tokens, then its every token followed by how many times it appears in a list. Note that this only shows the top 100 tokens.
with open(sys.argv[2]+"-tokens.txt", "r") as f:
    with open(sys.argv[2]+"-stats.txt", "w") as out:
        out.write(f"{total_tokens}\n")
        out.write(f"{len(unique_tokens)}\n")
        token_counts = {}
        for line in f:
            tokens = line.split()[1:] 
            for token in tokens:
                token_counts[token] = token_counts.get(token, 0) + 1
        sorted_tokens = sorted(token_counts.items(), key=lambda x: (-x[1], x[0]))
        total_tokens = 0
        unique_tokens = set()
        for token, count in sorted_tokens[:100]:
            out.write(f"{token} {count}\n")
