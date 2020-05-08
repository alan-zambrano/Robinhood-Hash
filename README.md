# Robin-Hood-Hash
dictionary.txt taken from https://github.com/dwyl/english-words/blob/master/words.txt

This program is an exercise intended to compare the run times of unordered_map
and robin hood hashing.

# Hash Function
My intention with this project was to hash a dictionary, as it contains a large
amount of unique strings. The uniqueness of the strings allowed me to use the
words as keys, which made hashing easy and ensured an even distribution for a
good hash function. Thus, in selecting a hash function, I only had to ensure
it sufficiently distributed words evenly across the table. There are plenty of
good hash functions, but for my purposes I found that Bob Jenkins'
one_at_a_time hash function did just that.

# Dynamic Resizing
Although I could have hard-coded a table size that would allow me to hash the
dictionary and not one item more, I decided to add some reusability. The table
by default starts very small (67) so that hashing a few elements doesn't reserve
300000 elements' worth of space. When the table reaches a load factor of .75
(the table is 3/4 full) the table roughly doubles in size. It will double until
it reaches a size of ~1M. These are rough estimates because the sizes are
predetermined so that they are prime. 1000000 is obviously not prime, so the
table will expand to the closest prime, 999983.

Unfortunately for the space saved using dynamic resizing, there is a cost. That
is, for the nth resize, there are in the worst-case 2^n + 2^(n-1) + 2^(n-2) +...
total elements hashed. One way to avoid this is by initializing the table to a
size that you know you won't exceed. When testing against unordered_map with
dynamic resizing was incredibly slow, so I avoided it in the results.

#Results
### Trial 1
|Operation         |RHH   |UMap  |RHH % faster|
|------------------|:----:|:----:|:----------:|
|hashing dictionary|148 ms|193 ms|23%         |
|deleting 1/3 elems|54 ms |45 ms |-16%        |
|rehashing 1/3     |45 ms |44 ms |-2%         |
|total time        |247 ms|282 ms|12%         |

### Trial 2
|Operation         |RHH   |UMap  |RHH % faster|
|------------------|:----:|:----:|:----------:|
|hashing dictionary|126 ms|199 ms|37%         |
|deleting 1/3 elems|63 ms |44 ms |-30%        |
|rehashing 1/3     |48 ms |46 ms |-4%         |
|total time        |237 ms|289 ms|17%         |

### Trial 3
|Operation         |RHH   |UMap  |RHH % faster|
|------------------|:----:|:----:|:----------:|
|hashing dictionary|120 ms|188 ms|36%         |
|deleting 1/3 elems|54 ms |45 ms |-17%        |
|rehashing 1/3     |44 ms |48 ms |8%          |
|total time        |218 ms|281 ms|22%         |

### Trial 4
|Operation         |RHH   |UMap  |RHH % faster|
|------------------|:----:|:----:|:----------:|
|hashing dictionary|117 ms|197 ms|41%         |
|deleting 1/3 elems|49 ms |46 ms |-6%         |
|rehashing 1/3     |41 ms |45 ms |9%          |
|total time        |207 ms|288 ms|28%         |

#Conclusion
When comparing total times, RHH seems to perform about 20% faster than UMap.
Given that, deletions are consistently slower. We may attribute this to the
hashing algorithm which involves shifting elements. This operation is costly and
the penalty is shown in the results. The rehashing operation-- in which we add
back the elements we removed in the previous operation-- is sometimes better and
sometimes worse. I attribute this to the relatively small sample size where run
times between the two are very similar
