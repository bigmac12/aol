#30301
No Good Align Tunnel Trig~
0 g 100
~
   wait 1s
   if (%actor.vnum% == -1)
   if ((%actor.align% < 1))
   msend %actor% A pair of well armored guards step forward and looks you over for a long moment.
   mechoaround %actor% A pair of well-armored guards pauses a moment, examining %actor.name% with great interest.
   wait 4s
   msend %actor% They step back and allow you to pass.
   mechoaround %actor% %actor.name% is allowed to pass down the tunnel.
   mteleport %actor% 30303
   wait 1s
   mforce %actor% look
else
   wait 2s
      say Greetings Traveler,
      wait 2s
      say This area is off limits, so turn back around.
   end
   end
~
#30302
leaving Sanction's Tunnel~
0 g 100
~
      wait 1s
      if (%actor.vnum% == -1)
      if ((%actor.align% < 1))
      msend %actor% A pair of well armored guards stares at you for a moment before one guard begins to unlock the gate.
      mechoaround %actor% A pair of well-armored guards glances at  %actor.name% before one guard begins to unlock the gate.
      wait 2s
      msend %actor% Pushing the gate open, he steps back as you to pass.
      mechoaround %actor% %actor.name% steps through the gate, out into the cold.
      mteleport %actor% 8445
      wait 1s
      mforce %actor% look
~
#30369
City Hall Greeting~
0 g 100
~
   if (%direction% == west)
   if (%actor.clan% == 1)
      wait 1s
      say Welcome to Sanctions City Hall
      mechoaround %actor% %actor.name% leaves west.
      msend %actor% The Guard ushers you into the building.
      mteleport %actor% 30473
      mechoaround %actor% %actor.name% has arrived from the west.
      mforce %actor% look
else
   wait 1s
      say Greetings Citizen,
      wait 2s
      say City Hall is closed to the general public today.
   end
   end
~
$~
