#10100
Mailman check mail~
0 d 0
Have I any mail?~
mforce %actor% check
~
#10101
Mailman receive mail~
0 d 0
I would like my mail~
mforce %actor% receive
~
#10102
Mailman mailing trigger~
0 d 0
I would like a stamp~
wait 1s
emote nods.
wait 3
say And to whom would you like to mail your letter to?
set %mailnum% 1
global %mailnum% 1
~
#10103
Mailman get name trigger~
0 d 0
.~
global %mailnum%
if (%mailnum% == 1)
  emote grins.
  unset mailnum
  mforce %actor% mail gilean
end
~
#10104
Ledborgian Greet~
0 g 100
~
If (%actor.vnum% == -1) then
   Wait 1s
   mecho Vendrikar Vendrikar says, 'Welcome to my shop, how can I help you?'
      If (%random.4% == 1) then
         wait 6s
mecho Vendrikar Vendrikar says, 'I'm watching you, keep away from those display cases!'
      else
      end
   else
   end
else
end
~
#10105
South GateGuardGreet~
0 g 100
~
if ((%actor.vnum% == -1) && (%direction% == south)) then
      wait 1s
      mecho guard The gate guard stiffens to attention as you approach and moves aside to allow entry into the city.
elseif ((%actor.vnum% == -1) && (%direction% == north)) then
      if (%actor.level% <=5) then
         wait 1s
         mecho guard The gate guard points down the road away from the city and says, 'Be wary, there are beings beyond these walls that prey upon the unprepared...remain always vigilant if you go.'
      else
         if (%random.2% == 1)
            wait 1s
            mecho guard The gate guard turns as you pass and glances down the road.
         else
            wait 1s
            mecho guard The gate guard shifts from one foot to another, sore from standing all day.
         end
      end
else
end
~
#10106
GuardCaptainGreet~
0 g 100
~
if (%actor.vnum% == -1) then
   set num %random.3%
      if (%num% == 1)
         wait 2s
         mecho captain The captain of the guard looks up as you approach.
         wait 3s
         mecho captain The captain of the guard says, 'Thats all I need...more distraction.'
      elseif (%num% == 2)
         wait 2s
        mecho captain The captain of the guard says, 'Where are those papers?!'
         wait 2s
         mecho captain The captain of the guard sighs loudly.
         wait 3s
         west
         wait 7s
         mecho captain The captain of the guard rumages through some papers.
         wait 3s
         mecho captain The captain of the guard says. 'Ahhh! Here they are!'
         wait 1s
         east
         wait 5s
         mecho captain The captain of the guard begins to read some papers.
      else
         wait 2s
         mecho captain The captian of the guard rubs his finger along the edge of his sword and smiles.
      end
else
end
~
#10107
West GateGuardGreet~
0 g 100
~
if ((%actor.vnum% == -1) && (%direction% == west)) then
      wait 1s
      mecho guard The gate guard stiffens to attention as you approach and moves aside to allow entry into the city.
elseif ((%actor.vnum% == -1) && (%direction% == east)) then
      if (%actor.level% <=5) then
         wait 1s
         mecho guard The gate guard points down the road away from the city and says, 'Be wary, there are beings beyond these walls that prey upon the unprepared...remain always vigilant if you go.'
      else
         if (%random.2% == 1)
            wait 1s
            mecho guard The gate guard turns as you pass and glances down the road.
         else
            wait 1s
            mecho guard The gate guard shifts from one foot to another, sore from standing all day.
         end
      end
else
end
~
#10108
North GateGuardGreet~
0 g 100
~
if ((%actor.vnum% == -1) && (%direction% == north)) then
      wait 1s
      mecho guard The gate guard stiffens to attention as you approach and moves aside to allow entry into the city.
elseif ((%actor.vnum% == -1) && (%direction% == south)) then
      if (%actor.level% <=5) then
         wait 1s
         mecho guard The gate guard points down the road away from the city and says, 'Be wary, there are beings beyond these walls that prey upon the unprepared...remain always vigilant if you go.'
      else
         if (%random.2% == 1)
            wait 1s
            mecho guard The gate guard turns as you pass and glances down the road.
         else
            wait 1s
            mecho guard The gate guard shifts from one foot to another, sore from standing all day.
         end
      end
else
end
~
#10109
Fish(Working_test_trigger_works_but_dont_use)~
2 c 100
fish~
if (%actor.vnum% == -1)
   if (%arg% != rod)
      wsend %actor% What do you want to use to fish?
   else
      wsend %actor% You heave back the rod and cast the line deep out into the water.
      wechoaround %actor% %actor.name% casts the line from the fishing rod far out into the water.
      set num %random.2%
      if (%num% == 1)
         wait 2s
         wsend %actor% A fish quickly bites at your hook and is easily hefted up out of the water.
         wechoaround %actor% %actor.name% reels in the line and hauls out a small fish.
         wload obj 9808
      else
         wait 2s
         wsend %actor% You reel back in your line empty and wet.
         wechoaround %actor% %actor.name% reels in a still empty fishing line.
else
   end
   end
~
#10110
m10102 Thief Greet~
0 g 100
~
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
emote ceases his practice, turning to look you over.
wait 4s
say Who are you? You're trespassing. I'd suggest you leave.
wait 4s
emote goes back to practicing combat movements, paying you scarce attention.
else
wait 3s
emote ceases his practice, turning to look you over.
wait 4s
say Ahh, a newcomer. Welcome to the Guildhouse, it isn't much but it suits our uses.
wait 4s
say I can teach you some basic skills, if you'd like. If you want advanced instruction, you'd be better off heading out to Palanthas.
wait 6s
say You can find the entrance in an alley between Penny and Wall streets.
wait 4s
emote stands, waiting to instruct you.
end
~
#10118
Pepe's acorn trigger~
0 g 100
~
wait 1s
if (%random.3% == 1)
  sigh
wait 2s
  say No, I DON'T sell acorns!
done
~
#10119
m10112 greet message~
0 g 100
~
if (%direction% == south)
if (%actor.clan% == 1)
   wait 1s
   say Welcome to Kalaman garrison
   mechoaround %actor% %actor.name% leaves north.
   msend %actor% The Knight ushers you into the garrison.
   mteleport %actor% 10123
   mechoaround %actor% %actor.name% has arrived from the south.
   mforce %actor% look
elseif (%actor.clan% == 5)
   wait 1s
   say Welcome to Kalaman garrison
   mechoaround %actor% %actor.name% leaves north.
   msend %actor% The Knight ushers you into the garrison.
   mteleport %actor% 10123
   mechoaround %actor% %actor.name% has arrived from the south.
   mforce %actor% look
else
wait 1s
   say Greetings Citizen
   wait 1s
   say The garrison is off limits to the general public
end
end
~
#10120
r10100 press button~
2 c 100
press~
if (%actor.vnum% == -1) then
if ("%arg%" != "button")
wsend %actor% Press what?
else
wsend %actor% You press the button and nothing seems to happen.
wechoaround %actor% %actor.name% presses the button and nothing seems to happen.
wait 3s
wecho The ground begins to vibrate
wait 3s
wecho You hear a screeching sound coming from beyond the door.
wait 3s
wecho A small bell rings and the door slides open.
wdoor 10100 west room 10117
wdoor 10117 east room 10100
wait 15s
wecho The door slides shut.
wdoor 10100 west purge
wdoor 10117 east purge
end
end
~
#10121
r10117 press buttons~
2 c 100
press~
if (%actor.vnum% == -1) then
if ("%arg%" == "1")
wsend %actor% You press the button and nothing seems to happen.
wechoaround %actor% %actor.name% presses the button and nothing seems to happen.
wait 3s
wecho The ground begins to vibrate
wait 3s
wecho You hear a screeching sound coming from beyond the room.
wait 3s
wecho A small bell rings, the door slides open and closes again.
wait 5s
wecho The door slides open.
wdoor 10119 west room 10117
wdoor 10117 east room 10119
wasound A small bell rings and the door slides open.
wait 10s
wasound The door slides shut.
wecho The door slides shut.
wdoor 10119 west purge
wdoor 10117 east purge
elseif ("%arg%" == "2")
wsend %actor% You press the button and nothing seems to happen.
wechoaround %actor% %actor.name% presses the button and nothing seems to happen.
wait 3s
wecho The ground begins to vibrate
wait 3s
wecho You hear a screeching sound coming from beyond the room.
wait 3s
wecho A small bell rings, the door slides open and closes again.
wait 5s
wecho The door slides open just enough to let you squeeze through.
wdoor 10120 west room 10117
wdoor 10117 east room 10120
wasound A small bell rings and the door slides open.
wait 10s
wasound The door slides shut.
wecho The door slides shut.
wdoor 10120 west purge
wdoor 10117 east purge
elseif ("%arg%" == "3")
wsend %actor% You press the button and nothing seems to happen.
wechoaround %actor% %actor.name% presses the button and nothing seems to happen.
wait 3s
wecho The ground begins to vibrate
wait 3s
wecho You hear a screeching sound coming from beyond the room.
wait 3s
wecho A small bell rings, the door slides open and closes again.
wait 5s
wecho The door slides open.
wdoor 10118 west room 10117
wdoor 10117 east room 10118
wasound A small bell rings and the door slides open.
wait 10s
wasound The door slides shut.
wecho The door slides shut.
wdoor 10117 east purge
wdoor 10118 west purge
elseif ("%arg%" == "4")
wsend %actor% You press the button and nothing seems to happen.
wechoaround %actor% %actor.name% presses the button and nothing seems to happen.
wait 3s
wecho The ground begins to vibrate
wait 3s
wecho You hear a screeching sound coming from beyond the room.
wait 3s
wecho A small bell rings, the door slides open and closes again.
wait 5s
wecho The door slides open.
wdoor 10100 west room 10117
wdoor 10117 east room 10100
wasound A small bell rings and the door slides open.
wait 10s
wasound The door slides shut.
wecho The door slides shut.
wdoor 10100 west purge
wdoor 10117 east purge
elseif ("%arg%" == "button")
wsend %actor% Which button?
else
wsend %actor% Press what?
end
end
~
#10122
r10119 press button~
2 c 100
press~
if (%actor.vnum% == -1) then
if ("%arg%" != "button")
wsend %actor% Press what?
else
wsend %actor% You press the button and nothing seems to happen.
wechoaround %actor% %actor.name% presses the button and nothing seems to happen.
wait 3s
wecho The ground begins to vibrate
wait 3s
wecho You hear a screeching sound coming from beyond the door.
wait 3s
wecho A small bell rings and the door slides open.
wdoor 10119 west room 10117
wdoor 10117 east room 10119
wasound A small bell rings and the door slides open.
wait 15s
wasound The door slides shut.
wecho The door slides shut.
wdoor 10119 west purge
wdoor 10117 east purge
end
end
~
#10123
r10118 press button~
2 c 100
press~
if (%actor.vnum% == -1) then
if ("%arg%" != "button")
wsend %actor% Press what?
else
wsend %actor% You press the button and nothing seems to happen.
wechoaround %actor% %actor.name% presses the button and nothing seems to happen.
wait 3s
wecho The ground begins to vibrate
wait 3s
wecho You hear a screeching sound coming from beyond the door.
wait 3s
wecho A small bell rings and the door slides open.
wdoor 10118 west room 10117
wdoor 10117 east room 10118
wasound A small bell rings and the door slides open.
wait 15s
wasound The door slides shut.
wecho The door slides shut.
wdoor 10118 west purge
wdoor 10117 east purge
end
end
~
#10124
r10120 Steam Pulse~
2 g 100
~
if (%actor.vnum% == -1)
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.'
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.'
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.'
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.'
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.'
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.'
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.'
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.'
wait 2s
wecho The machine groans and a jet of steam shoots forth burning you.'
end
~
#10125
r10120 Press Button~
2 c 100
press~
if (%actor.vnum% == -1) then
if ("%arg%" != "button")
wsend %actor% Press what?
else
wsend %actor% You press the button and nothing seems to happen.
wechoaround %actor% %actor.name% presses the button and nothing seems to happen.
wait 3s
wecho The ground begins to vibrate
wait 3s
wecho You hear a screeching sound coming from beyond the door.
wait 3s
wecho A small bell rings and the door slides open.
wdoor 10120 west room 10117
wdoor 10117 east room 10120
wasound A small bell rings and the door slides open.
wait 15s
wasound The door slides shut.
wecho The door slides shut.
wdoor 10120 west purge
wdoor 10117 east purge
end
end
~
#10126
m10157 smile and greet~
0 g 100
~
if (%actor.vnum% == -1) then
wait 1s
say Hi!Howareyoudoing?
end
~
#10150
Palanthas Mage Door Open~
2 d 100
Savak~
if %actor.vnum% == -1
if %actor.class% == mage
wait 3
wecho The secret door slides open silently.
wdoor 2018 west room 2058 flags a
wdoor 2058 east room 2018 flags a
end
end
~
#10151
Palanthas Mage Door Close~
2 d 100
Sorak~
if %actor.vnum% == -1 and
if %actor.class% == mage
wait 3
wecho The secret door slides shut with a slight hissing sound.
wdoor 2018 west purge
wdoor 2058 east purge
~
#10152
Palanthas Mage door Final CLose~
2 g 100
~
if %actor.vnum% == -1
if %direction% == east
wait 5s
wdoor 2018 west purge
wdoor 2058 east purge
wecho The secret door shuts behind you quietly.
~
#10153
Palanthas Mage Teleport~
2 g 100
~
if %actor.vnum% == -1
if %direction% == east
if %actor.class% != mage
wait 2
wecho The air shimmers slightly and you feel disoriented.
wteleport %actor% 1900
~
#10196
Mage Room teleport~
2 g 100
~
if (%actor.vnum% == -1)
if (%actor.class% != mage)
wait 3
wecho The air shimmers faintly.
wait 5
wteleport %actor% 10163
else
wait 2
wecho The surface of the pool ripples slightly.
end
end
end
~
#10197
Mage Door Final Close~
2 g 100
~
if ((%actor.vnum% == -1) & (%direction% == east)) 
wait 5s
wecho The door slides shut suddenly.
wdoor 10103 east purge
wdoor 10198 west purge
end
~
#10198
Mage Door Close~
2 d 100
Sorak~
if (%actor.vnum% == -1)
if (%actor.class% == mage)
wait 2
wecho The secret door slides shut quietly.
wdoor 10103 east purge
wdoor 10198 west purge
else
wait 2
wecho The words echo hollowly.
end
end
end
~
#10199
mage Door Open~
2 d 100
Savak~
if (%actor.vnum% == -1)
if (%actor.class% == mage)
wait 2
wecho The secret door slides open silently.
wdoor 10103 east room 10198 flags a
wdoor 10198 west room 10103 flags a
else
wait 2
wecho The words echo hollowly.
end
end
end
~
$~
