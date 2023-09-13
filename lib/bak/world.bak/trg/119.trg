#11900
For Room 11900 - Gate Trigger~
2 b 50
~
wait 3
wecho A soft wind blows the gate open, causing it to creek noisily!
wait 8s
wecho  The soft wind dissipates leaving all quiet once again.
~
#11901
For Room 11903 - Burst of Steam~
2 g 100
6~
If (%actor.vnum% == -1) then
wait 2s
wsend %actor% A burst of hot steam scorches you!
wdamage %actor% 25
endif
~
#11902
For most rooms - Glowing wall trigger~
2 b 65
~
wecho The walls begin to glow brightly!
wait 5s
wecho The light slowly fades out.
~
#11903
etheral trigger - chill factor~
1 j 100
~
if (%actor.vnum%== -1) then
wait 1
osend %actor% A sudden chill overcomes you, as it spreads throughout your being.
wait 1
oechoaround %actor% A white mist comes pouring into this room.
endif
~
#11904
Mobtrigger #11909~
0 g 100
~
if (%actor.vnum%== -1) then
wait 1
say Greetings Adventures, You must choose your path. Define your charecter in a different perspective.
say Please read the three plaques on each statue before choosing either Honor, Valor, or Poverty
endif
~
#11905
choice trigger for mob 11909~
0 d 100
Valor~
if (%actor.vnum%== -1) then
sa Wise Choice adventure
msend %actor% In a binding flash of white light you are gone!
Mteleport %actor% 11954
~
#11906
trigger for mob 11910~
0 b 50
~
say The path of valor leads to ones vanity adventurers
sa Tread carefully down this path, or it will lead to your death. The beast that lies beneath these walls will feed off your Valor
~
#11907
trigger for object 11919 - Command word~
1 j 100
~
if (%actor.vnum%==-1) then
osend %actor% Visions of dishonor and hate fills your mind.
dg_cast 'bless' %actor%
Oechoaround The swords emits a faint eerie glow!
endif
~
#11908
for the the wall trigger~
0 h 100
~
say My trigger commandlist is not complete!
~
$~
