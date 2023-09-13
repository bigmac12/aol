#9900
z99 Pain and Suffering (assigned to whole temple of Pal)~
2 g 90
~
if (%actor.align% < 0) & (%actor.vnum% == -1) then
wait 1
wsend %actor% Your body is racked with pain warning you that your presence here is not wanted!
wechoaround %actor% %actor.alias% suffers from a malady of unknown origins.
wdamage %actor% 10
end
~
#9970
Marble platform slide trigger~
2 c 100
climb~
   if (%actor.vnum% == -1)
if (%arg% == platform)
wsend %actor% You climb up the platform.
wechoaround %actor% %actor.name% climbs up the platform.
wait 2s
wsend %actor% Nearing the edge, you sit on the platform and slide down it.
wechoaround %actor% %actor.name% sits on the edge of the platform and slides down it.
wait 2s
switch %random.10%
case 10
wsend %actor% You land with a large splash into the water, sending waves over the bath.
wechoaround %actor% %actor.name% lands with a large splash into the water, sending waves over the bath.
break
case 9
wsend %actor% You splash into the water, sending large ripples across the surface.
wechoaround %actor% %actor.name% splashes into the water, sending large ripples across the surface.
break
case 8
wsend %actor% You splash into the bath headfirst, catching a mouthful of water.
wechoaround %actor% %actor.name% splashes into the bath headfirst, catching a mouthful of water.
break
case 7
wsend %actor% You slip off the platform and slide underneath the water with barely a splash.
wechoaround %actor% %actor.name% slips off the platform and slides underneath the water with barely a splash.
break
default
wsend %actor% You land with a splash into the water.
wechoaround %actor% %actor.name% lands with a splash into the water.
break
done
end
end
~
#9982
m9982 I've got wood.~
0 f 100
~
if (%random.10% == 1)
mload obj 9982
end
~
$~
