workspace/intro

This algo computes the crossing time and total distance
The assumption is made that a faster hiker on the right side of the bridge
can't help the hikers on the left side to improve the crossing time.
Each group on the left side independently has to cross the bridge and
then join the group of hikers on the right side.

[constantin.musteata@localhost intro]$ ./run.sh
open file: intro/intro.yaml
bridge 0: [100 ft, 17 min]
bridge 1: [250 ft, 132.5 min]
bridge 2: [150 ft, 95.5 min]
overall : [500 ft, 245 min]
begin time: Tue Aug 20 16:08:40 2019
  end time: Tue Aug 20 16:08:40 2019
   elapsed: 0.000279383 seconds

Obs: this algorithm is using std::vector as underlying data-structure for
     moving the hikers accross the bridge

workspace/intro2

This algo computes the crossing time and total distance
The assumption is made that a faster hiker on the right side
may help the hikers on the left side to improve the crossing time
After each pair-crossing from the left side to the right side
the ranking of the hikers is reevaluated in order to establish
the fastest hiker to return the torch back to the left side.

[constantin.musteata@localhost intro2]$ ./run.sh
open file: intro2/intro.yaml
bridge 0: [100 ft, 17 min]
bridge 1: [250 ft, 132.5 min]
bridge 2: [150 ft, 95.5 min]
overall : [500 ft, 245 min]
begin time: Tue Aug 20 16:08:40 2019
  end time: Tue Aug 20 16:08:40 2019
   elapsed: 0.000471861 seconds

Obs: this algorithm is using std::deque as underlying data-structure for
     moving the hikers back and forth accross the bridge

