#5100
r5196 mage door open~
2 d 1
Savak~
if (%actor.vnum% == -1)
      if (%actor.class% == mage)
   wait 2
         wecho The secret door slides open silently.
wdoor 5196 west room 5198 flags a
wdoor 5198 east room 5196 flags a
      else
   wait 2
         wecho The words echo hollowly.
         end
   end
~
#5101
r5196 mage door shut~
2 d 1
Sorak~
 if (%actor.vnum% == -1)
      if (%actor.class% == mage)
         wait 2
         wecho The secret door slides shut quietly.
wdoor 5196 west purge
wdoor 5198 east purge
      else
         wait 2
         wecho The words echo hollowly.
      end
   end
~
#5102
r5196 door autoshut~
2 g 100
~
if (%actor.vnum% == -1) then
if (%direction% == west) then
wait 5
wecho The door slides shut suddenly.
wdoor 5196 west purge
wdoor 5198 east purge
end
end
~
#5103
r5198 teleport out~
2 g 100
~
  if (%actor.vnum% == -1)
      if (%actor.class% != mage)
   wait 3
         wecho The air shimmers faintly.
         wait 5
wteleport %actor% 5082
      else
         wait 2
         wecho The surface of the pool ripples slightly.
      end
   end
~
#5130
Temp of Zeb !GOOD (whole temple)~
2 g 90
~
if (%actor.align% > 0) && (%actor.vnum% == -1) then
   wait 1
   wsend %actor% Your body is racked with pain warning you that your presence here is not wanted!
   wechoaround %actor% %actor.alias% suffers from a malady of unknown origins.
   wdamage %actor% 10
   end
~
#5187
r5187 Sacrifice trigger~
2 h 100
~
wait 1
wecho The flames crackle loudly as they consume all that has been dropped into the cauldron.
wpurge
wload obj 5197
~
#5197
m5195 collector drop/purge~
0 e 100
sells~
   say Please excuse me a moment.
wait 4
emote carries her new item upstairs to add to her collection.
up
drop all
down
wait 6
~
$~
