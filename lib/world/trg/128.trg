#12800
m12800 deathload of o12801, 12803, 12804, 12808~
0 f 100
~
eval objload %random.12%
if (%objload% == 1) then
mload obj 12803
elseif (%objload% == 2) then
mload obj 12803
elseif (%objload% == 3) then
mload obj 12804
else
mload obj 12808
end
~
#12801
m12800 aggro on rest~
0 c 100
rest~
return 1
mechoaround %actor% The sands shift and the rocks seem to rise up as though they have a life of their own.
msend %actor% The sands shift and the rocks seem to move in your direction.
wait 1s
mkill %actor%
~
#12802
sand moving or sandstorm~
2 g 100
~
If (%actor.vnum% == -1) then
wait 5
eval todamornot %random.5%
if (%todamornot% == 2) then
wsend %actor% A stiff breeze blows through the area stirring up a cloud of fine sand.
elseif (%todamornot% == 3) then
wsend %actor% A strong gust of wind sends grains of sand piercing into your flesh!!!
wdamage %actor% 10
wait 2s
wsend %actor% The fine grains of sand rip through your flesh!!!
wdamage %actor% 5
else return 1
end
end
~
#12803
m12800 aggro on sit~
0 c 100
sit~
return 1
   mechoaround %actor% The sands shift and the rocks seem to rise up as though they have a life of their own.
   msend %actor% The sands shift and the rocks seem to move in your direction.
   wait 1s
   mkill %actor%
~
#12804
m12800 aggro on sleep~
0 c 100
sleep~
return 1
   mechoaround %actor% The sands shift and the rocks seem to rise up as though they have a life of their own.
   msend %actor% The sands shift and the rocks seem to move in your direction.
   wait 1s
   mkill %actor%
~
#12805
prevent scanning trigger~
2 c 100
scan~
wsend %actor% The heat haze rising from the ground here prevents you from seeing anything at any distance.
~
#12806
lion death load~
0 f 100
~
 eval objload %random.5%
if (%objload% == 2) then
mload obj 12805
elseif (%objload% == 3) then
mload obj 12806
else return 1
end
~
#12807
afraid of hatori so flee trigger~
0 k 40
~
msend %actor% You are overcome with fear.
mechoaround %actor% %actor.alias% gets a panicked look.
mforce %actor% flee
~
#12808
move stones trigger for rooms 12805, 14 and 15~
2 c 100
move~
if (%actor.vnum% == -1) then
   if ("%arg%" == "stones") then
   wsend %actor% You begin to move the pile of stones.
   wechoaround %actor% %actor.name% starts to move the pile of stones.
   wait 3s
   eval mobload %random.5%
   if (%mobload% == 2) then
   wechoaround %actor% %actor.name% has discovered a pile of coins!
   wsend %actor% You have found a pile of coins!
   wload obj 12807
   elseif (%mobload% == 3) then
   wechoaround %actor% %actor.name% has disturbed a fire ant mound!
   wsend %actor% You have disturbed a fire ant mound!
   wload mob 12806
   wload mob 12806
   wload mob 12806
   elseif (%mobload% == 4) then
   wechoaround %actor% %actor.name% has discovered a pile of coins!
   wsend %actor% You have found a pile of coins!
   wload obj 12807
   else
   wsend %actor% You finish moving the stones but discover nothing.
   wechoaround %actor% %actor.name% finishes moving the stones but does not find anything.
   return 1
   end
else
   wsend %actor% Move what?
end
end
end
~
#12809
madman warning about rocks in r12806~
0 g 100
~
if (%actor.vnum% == -1)
wait 1s
say Don't rest! Never Rest!
wait 1s
say NEVER REST NEAR THE ROCKS!!!
else return 1
end
~
#12820
m12820 Talasha Teleports Around~
0 b 100
~
wait until 2200
wait 1s
yawn self
wait 2s
emote turns to her men and salutes.
wait 2s
say I'm beat.  You take the evening watch, keep an eye out for trouble.
wait 3s
mechoaround talasha With that, the commander does an about turn and heads to the comfort of her tent.
wait 1s
mteleport self 12838
wait 3s
emote enters the tent and unbuckles her armor as she prepares for bed.
wait 1s
close flap
wait 3s
yawn self
wait 2s
say You coming to bed, Tabarre?
wait 2s
grin tabarre
wait 6s
rest
wait 4s
emote pats the cot beside her and curls up in a blanket.
wait 5s
kiss tabarre
wait 2s
yawn
wait 3s
sleep
wait 10s
snore
wait until 0500
wait 2s
wake
wait 3s
smile tabarre
wait 2s
kiss tabarre
wait 4s
stand
wait 3s
emote stretches and yawns.
wait 4s
emote whispers tenderly to Tabarre.
wait 5s
emote quietly puts her armor back on before leaving the tent.
wait 3s
open flap
wait 2s
ne
wait 1s
mteleport self 12839
wait 1s
emote climbs up onto the walls of the pallisade, salutes her men, and returns to duty for the day.
halt
halt
~
#12821
m12822 Tabarre Teleports Around~
0 b 100
~
wait until 2200
wait 1s
yawn self
wait 2s
emote looks to the south, a small grin spreading across his face.
wait 6s
emote walks toward the large tent in the campsite.
wait 7s
mteleport self 12838
wait 5s
nod talasha
wait 4s
emote goes to the bed and starts stripping off some of his armor.
wait 4s
grin talasha
wait 4s
kiss talasha
wait 2s
emote lies down on the bed beside the commander, wrapping his arms around her.
wait 3s
sleep
wait until 0600
wait 2s
wake
wait 3s
emote looks about, bleary-eyed.
wait 2s
emote slowly stands up from the bed.
wait 4s
stand
wait 3s
emote stretches and yawns.
wait 4s
emote quietly puts his armor back on, looking to the cot with a slight grin.
wait 3s
open flap
wait 2s
ne
wait 1s
mteleport self 12835
wait 1s
yawn self
halt
halt
~
#12822
m12820 Talasha Armor Death~
0 f 100
~
eval objload %random.16%
if (%objload% == 1) then
mload obj 12820
elseif (%objload% == 2) then
mload obj 12821
elseif (%objload% == 3) then
mload obj 12824
elseif (%objload% == 4) then
mload obj 12825
else
return 0
end
~
#12823
m12822 Tabarre Eq Deathload~
0 f 100
~
eval objload %random.8%
if (%objload% == 1) then
mload obj 12829
elseif (%objload% == 2) then
mload obj 12839
else
return 0
end
~
#12830
m12830 Fucko Cry Trigger~
0 d 100
Fucko~
set num %random.3%
if (%num% == 1)
wait 2s
sniff
wait 2s
:slouches off, crying.
wait 1s
n
ne
nw
w
flee
flee
flee
flee
elseif (%num% == 2)
wait 2s
:narrows his eyes.
wait 2s
think
wait 5s
say Takes one... too... uh... nobody!
wait 2s
emote laughs stupidly.
elseif (%num% == 3)
wait 2s
sulk
wait 4s
say You're mean.
wait 2s
glare
end
~
#12838
m12838 Fat Guy Laughing and Eating~
0 b 38
~
set num %random.5%
if (%num% == 1) then
wait 4s
laugh
wait 9s
emote snorts with obnoxious laughter.
wait 5s
emote stops laughing just long enough to stuff another fistful of venison into his mouth.
wait 10s
emote bursts out laughing again, causing partially-chewed venison to spray everyone in the area.
wait 6s
elseif (%num% == 2) then
wait 2s
rofl bitter
wait 8s
chuck
wait 10s
laugh
wait 4s
elseif (%num% == 3) then
wait 4s
emote stuffs a huge hunk of meat into his face.
wait 9s
emote chews noisily, his mouth wide open.
wait 4s
mechoaround slovenly some grease runs down the fat mercenary's beard.
wait 7s
elseif (%num% == 4) then
wait 9s
emote bursts out with laughter, so loud and uncontrollable that he isn't able to take another bite for a few moments.
wait 8s
emote resumes eating in his own disgusting fashion.
wait 5s
elseif (%num% == 5) then
wait 2s
say Huh?  What?
wait 6s
emote stares blankly for a moment.
wait 7s
say What was I doing?
wait 8s
shrug self
wait 4s
emote resumes eating his fistfuls of meat, laughing periodically.
wait 5s
end
~
#12839
m12839 Bitter Old Man Trigger~
0 b 27
~
set num %random.4%
if (%num% == 1) then
say Hehehe.  You hear the one about the two halfelf half-brothers?
wait 4s
emote laughs, coughing violently afterward for a long moment.
wait 7s
emote clears his throat.
wait 3s
say What was I sayin'?
wait 4s
say Oh yeah, the two halfelf half-brothers...
wait 6s
frow me
wait 4s
say That was a great story.  Hey... you hear the one about the two halfelf half-brothers?
wait 8s
elseif (%num% == 2) then
wait 2s
emote scowls at a passing recruit.
wait 2s
say Hey!  Watch that weapon, you're gonna put someone's eye out carrying it like that!
wait 5s
emote mutters something to himself, rubbing his sore knee.
wait 7s
say Stupid kids.  Don't know nothin' about nothin'.
wait 9s
elseif (%num% == 3) then
wait 2s
:spits into an upturned helmet on the ground.
wait 3s
emote looks up at you, grinning conspiratively.
wait 5s
emote whispers to you, 'Don't worry.  It's his.'
wait 2s
point slovenly
wait 3s
grin me
wait 5s
emote laughs, which erodes into another fit of coughing.
wait 4s
elseif (%num% == 4) then
wait 3s
say Know what I heard?
wait 6s
say I heard our illustrious commander's got her eye on someone new!
wait 5s
say I hope whoever it is, he's got more stamina than her last favorite, hehehee!
wait 9s
say Hey, gimme that damn venison.  You're gonna choke to death.
wait 10s
end
~
#12840
Stupid Elves! trigger~
0 g 100
~
if (%actor.vnum% == -1) & (%actor.race% == qualinesti)
wait 2s
say Bloody pointy-eared freaks.
wait 4s
growl
wait 2s
say What are you starin' at, elfskin?
wait 3s
curse %actor%
elseif (%actor.vnum% == -1) & (%actor.race% == silvanesti)
wait 2s
mutter %actor%
wait 2s
say Blasted stiff-nosed elven creeps.
wait 3s
say Yeah I'm talkin' about you!
wait 4s
say Git outta here!  You ain't wanted!
elseif (%actor.vnum% == -1) & (%actor.race% == kagonesti)
wait 2s
snicker self
wait 3s
say Painty-skinned barbarian elves.
wait 2s
say Go back where you came from!
wait 2s
curse %actor%
elseif (%actor.vnum% == -1) & (%actor.race% == halfelven)
wait 2s
peer %actor%
wait 2s
frow
wait 2s
say Half-breed bastard.  Git outta my sight.
wait 3s
snarl %actor%
end
~
#12860
m12860 test greet~
0 g 100
~
if (%actor.vnum% == -1) then
wait 1s
say Oh! Hello,youmustbethereplacementtester. Apleasurehavingyouonboardjustheadsouthplease!
wait 2s
emo smiles and motions to the south before turning back to his parchments.
end
~
#12861
r12861 gnomeflinging incident~
2 g 100
~
if (%actor.vnum% == -1) then
wait 1
wecho A gnome dressed in a leather jacket and a flowing white scarf climbs into the basket of the monstrous trebuchet.
wait 2 s
wecho One of the many gnomes gathered on the ground calls up to the gnome in the trebuchet, '&CREADY?&n'
wait 2 s
wecho The gnome in the trebuchet gives a thumbs up to the gnomes gathered on the ground.
wait 1 s
wecho The gnomes on the ground scramble to set the trebuchet in motion.
wait 3 s
wecho Finally one of the gnomes triggers the activation mechanism and the trebuchet goes off!
wecho The arm swings forward slowly at first and then picks up speed. The gnome is pitched forward, straight into the ground.
wait 2 s
wecho The gnomes gathered on the ground scramble towards the injured gnomes. '&CAmazing! Quicklygettohimbeforehelosesconsciousness!&n' '&CSomeonecontacttheDepartmentofMedicinalScience! Wehaveanewtestsubjectforthem!&n' '&CNonononono! Firstgetallthedata!&n' 
end
~
#12862
m12861 bizzy greet~
0 g 100
~
if (%actor.vnum% == -1) then
wait 13 s
say Oh! Hello,soyou'rethenewonearen'tyou? Afterthelastoneexpiredonusit'sbeenalittlehardgettingreplacements...
wait 2 s
say Anyway,what'syourname? Oh,that'snotreallyimportantnowisit? Let'sgetstarted,justclimbaboardthesmallone.
smile
end
~
#12863
r12861 tele all the way to r12866~
2 c 100
board~
if (%actor.vnum% == -1)
if (%arg% == trebuchet)
   wsend %actor% You climb aboard the basket of the smaller trebuchet.
   wechoaround %actor% %actor.name% climbs aboard the smaller trebuchet.
   wait 2 s
   wecho The gnomes scramble to start the machine.
wait 2 s
   wecho Finally a gnome pulls on the triggering lever.
   wait 1 s
   wsend %actor% The basket shoots forward, propelling you into the air.
   wechoaround %actor% %actor.name% is suddenly shot into the air by the machine.
wait 1 s
   wteleport %actor% 12867
   wforce %actor% look
wait 1 s
   wsend %actor% You are flying through the air at an amazing speed.
wait 3 s
wteleport %actor% 12868
   wforce %actor% look
wait 1 s
   wsend %actor% You finally reach the top of your ascent and feel yourself plummet down to the ground. Maybe this wasn't such a good idea after all.
wait 3 s
   wteleport %actor% 12866
wforce %actor% look
wait 1 s
   wsend %actor% You land roughly into the stack of hay covering the clearing.
   wechoaround %actor% %actor.name% flies out of the air and lands directly into a large pile of hay. Straws of hay fly everywhere.
   end 
   end
~
#12894
r12894 Entry Trigger~
2 g 100
~
If (%actor.vnum% == -1) then
wait 2
wecho A bright flash is expelled from the east.
~
$~
