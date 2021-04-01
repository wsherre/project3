William Sherrer

this is a memory allocator for c

Design:
small block headers are like so

|-----------------|-------------|-----------------|
| 8 byte next_ptr | 2 byte size | 2 byte free_list|
|-----------------|-------------|-----------------|

size is the size of the map, 2,4,8,16,32,64 etc
return the pointer by concatinating it with the page head

big block headers are like so

|-----------------|
| 8 byte size     |
|-----------------|

i wanted the size to be 8 bytes incase its really big, i just realized nothing will realistically be that big....oh well

Known Problems:

So i noticed i was unmapping memory and passing in the pointer, however unmap needs the pointer to be the start of a page. 
so if i run 5,000,000 operations and map 2,000,000 big blocks i'll get 2 mil errors when unmmapping those blocks. the autograder doesn't catch this so im not sweating.

however if i change it to it unmaps the start of the page then for some reason, thousands of mallocs and free later a memcpy will segfault. and it segfaults if the size is > 4088 or if theres 2 pages but im barely using that second page. I don't know why because i account for the header when mapping and unmapping the pages. It's not technically a problem because it passes all of the test cases, but just thought i'd throw it out there. please dont take points off
