## MapReduceWebScraper

### What is it?

This utility is designed to check url pages that can be achieved from given urls with limited depth for searching.

It takes an input file, that contain links, and max depth.

The program returns links, that can be achieved using no more than $depth clicks on links.

The utility is based on famous Map-Reduce technology for distributed computations. So, it can be runned on high-level server machines and work simultaneously, using all available resources.

### So, how to use that?
First of all, we need to compile binaries. If you trust me, you can move compiled on my system binaries from **Build** folder. 
Otherwise, you can run bash script:
```commandline
bash BuildScripts.sh
```

And now we can use this using simple command:
```commandline
bash WebScraperMainScript input.txt output.txt depth
```
where
- **depth** is maximal depth of links.
- **input.txt** - file with links in format ```link\t1\n```.
- **output.txt** - file for results.


### How does it work?

The model can be simply described as BFS algorithms on links.

Script just run $depth times the map-reduce cycle.

1) **Map** splits input links on many files, and run a process for each that gets all accessable links from the given links. At the end, it merge all files in one.
 
2) **Reduce** sorts the file from previous step, then splits it into small, and make a final file with unique values.

### Utils
For generating simple dataset for testing I used **generate.cpp**:
```commandline
./generate number_of_links > file_name.txt
```
P.S. links are taken from "top-100 visited websites", so $number_of_links is no more than 100.
