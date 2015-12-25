#6200
Mage Door Open~
2 d 100
Savak~
if (%actor.vnum% == -1)
   if (%actor.class% == mage)
wait 2
      wecho The secret door slides open silently.
      wdoor 6281 west room 6290 flags a
      wdoor 6290 east room 6281 flags a
   else
wait 2
      wecho The words echo hollowly.
   end
end
~
#6201
Mage Door CLose~
2 d 100
Sorak~
if (%actor.vnum% == -1)
   if (%actor.class% == mage)
      wait 2
      wecho The secret door slides shut quietly.
      wdoor 6281 west purge
      wdoor 6290 east purge
   else
      wait 2
      wecho The words echo hollowly.
   end
end
~
#6202
Mage Door Close Final~
2 g 100
~
if ((%actor.vnum% == -1) && (%direction% == west)) 
    wait 5s
    wecho The door slides shut suddenly.
    wdoor 6281 west purge
    wdoor 6290 east purge
end
~
#6203
Mage Teleport~
2 g 100
~
if (%actor.vnum% == -1)
   if (%actor.class% != mage)
wait 3
      wecho The air shimmers faintly.
      wait 5
      wteleport %actor% 1900
   else
      wait 2
      wecho The surface of the pool ripples slightly.
   end
end
~
$~
