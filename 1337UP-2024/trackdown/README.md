# Trackdown
> There's a fugitive on the loose and we need to track him down! He posted this to social media recently, do you know where the 
> photograph was taken from? If you can provide the precise building, we can move in immediately 🚔
> Flag format: INTIGRITI{Location_Name}

## Solution
trying to reverse image search doesn't bring up anything super useful, but all hope is not lost.

we are given an image with a few key indicators of where the image was taken. The first thing that stands out is the “Trang Tien Plaza” sign, which is a shopping mall in Hanoi, Vietnam. 

Though just going off of "Trang Tien Plaza" gives a large area to work with, we can narrow it down by looking for the Rolex and Gucci stores in the image.

With this searched up on google maps we can find the correct street this was taken on, and then it was just a matter of finding the correct resteraunt. which i used the tables in the image to find the correct one.

which got us to this location: Si Lounge Hanoi

trying this out as the flag gives us the correct flag:

```shell
INTIGRITI{Si_Lounge_Hanoi}
```

