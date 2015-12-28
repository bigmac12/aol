#3236
r3236 Ferry~
2 g 100
~
if (%actor.name% == Halededanten)
   wait 2s
   wforce Halededanten say It is time to cast off! Get to work, my boys!
   wdoor 3236 east purge
   wait 2s
   wecho The great tortoises let out a snort as they begin their journey.
   wait 10s
   wecho The ferry glides away from the landing.
   wait 10s
   wecho The soothing sounds of the forest can be heard coming from all directions.
   wait 10s
   wecho The gentle lapping of the tortoise's flippers in the water joins the melody.
   wait 20s
   wforce Halededanten say We are nearly there, my boys!
   wait 20s
   wecho The ferry slowly glides up to the landing.
   wait 10s
   wecho The tortoises snort quietly as they take a short rest.
   wdoor 3236 north room 3200
   wasound The ferry arrives at its landing.
   wdoor 3236 north purge
   wait 2s
   wforce Halededanten say All ashore who is going ashore!
   wait 4s
   wecho Halededanten goes off to tend to the tortoises.
   wpurge Halededanten
   wteleport all 3200
   wforce all look
   wdoor 3236 north purge
~
$~
