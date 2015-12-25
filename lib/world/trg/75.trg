#7533
o7533 move rock to create down exit~
1 c 4
move~
if (%actor.vnum% == -1) then
if (%actor.str% >= 17) then
oecho %actor.str%
end
end
~
#7560
m7560 Final Ending Claw Chest Trigger Thing~
0 j 100
~
if (%object.vnum% == 7599) then
wait 1s
say The heroes have returned!
wait 2s
mechoaround helgrin The tavern erupts with wild cheering, dwarves all around clanging their cups on their tables and stomping their feet.
wait 3s
say Without you, we would not have known the evil existed amongst us, and without you, we certainly wouldn't have been able to destroy it.
wait 3s
smile %actor%
wait 2s
say We are eternally in your debt.
wait 2s
bow %actor%
wait 2s
say We haven't immense riches to offer, but... we do have something that I think you have earned through your selfless act.
wait 3s
:looks to his wife, who nods back.  With that, he pulls the heavy wooden chest out from behind the bar.
wait 4s
remo key
wait 1s
unlock chest
wait 1s
open chest
wait 1s
mforce %actor% get all chest
put claw chest
wait 1s
close chest
lock chest
wait 2s
emote puts the claw safely away where its evil will no more plague the world.
wait 3s
wear key
wait 1s
say Stay as our honoured guests for as long as you like.
wait 3s
say You shall always be welcome amongst our people.
wait 3s
emote raises his mug in salute, which is met with more cheers from the crowd.
wait 4s
mechoaround helgrin The next few hours are filled with round after round of free drinks, pats on the back, and boistrous congratulations from the grateful dwarves.
wait 4s
mechoaround helgrin You feel enriched by your deeds.
end
~
#7561
m7562 Breslin Farewell Trigger~
0 g 100
~
if (%actor.vnum% == -1)
wait 1s
say Leavin' already, aye?
wait 3s
:chuckles softly, nodding.
wait 3s
say Livin' under the mountain ain't for everyone.  Farewell, friend.
wait 2s
wave %actor%
wait 2s
mforce %actor% trigger2
end
~
#7562
m7562 Breslin Welcome Trigger~
0 g 100
~
if (%direction% == south) and (%actor.vnum% == -1) then
wait 1s
emote sits up slightly as you approach.
wait 3s
peer %actor%
wait 3s
say Ah... visitors!  By gar it's been a while...
wait 2s
ponder %actor%
wait 3s
say Haven't seen your kind here in... well, pretty near a few years at least.
wait 4s
say Well, old Breslin's a greeter, ain't no questioning that.
wait 3s
say Welcome, ye traveller, to the proud town o' Bitter Rock!
wait 3s
say Be our guests, but mind ye don't cause no troubles, ye hear?
wait 3s
cough self
wait 3s
say Beer Hall's always open.  Mardif, Bard Extraordinaire has extended his stay and will play ye a tune for a copper or three.
wait 5s
say Learn a thing 'er two at the guildhalls!
wait 4s
say An just maybe ye can get a tour o' the mines.  See how a real dwarfmine operates!
wait 3s
emote coughs dryly, doubling over.
wait 4s
emote regains his composure, frowning and just waving you in.
wait 3s
say Well, don't just stand there!  Come in!
wait 6s
emote waits a moment, then mutters something under his breath about you.
wait 6s
emote sits against the rocky wall again, not uttering a further word.
mforce %actor% trigger1
end
~
#7563
m7563 Greet Trigger~
0 g 25
~
if (%actor.vnum% == -1)
wait 1s
emote grins widely as you approach.
wait 2s
emote &Gsings&W, '&CCome buy from the seller, come by!&n'
wait 2s
snicker self
wait 2s
emote &Gsings&W, '&CTrinkets from the north!  Priced half their worth!&n'
wait 3s
emote &Gsings&W, '&CCome buy!!&n'
wait 2s
emote rubs his hands, eager as always to make the sale.
wait 100s
end
~
#7564
r7564 XYZ Trigger~
2 c 100
xyz~
wait 2s
wecho The poor dwarf can't even stand.  His legs have both been nearly eaten away, and he has lost much blood.  It is a miracle he is not already dead.
wait 4s
wecho &Wa dying dwarf &Gsays&W, '&CTh... thank Reorx... you've come...&n'
wait 3s
wecho The dwarf waves off offers of aid.
wait 3s
wecho &Wa dying dwarf &Gsays&W, '&CIt... it is too late for me...&n'
wait 3s
wecho His eyes widen slightly as a spasm of pain overtakes him.
wait 4s
wecho &Wa dying dwarf &Gsays&W, '&CEvil... in the mines... dwarves above must be... warned...&n'
wait 3s
wecho The dwarf produces a crumpled parchment, offering it to you.
wait 4s
wecho &Wa dying dwarf &Gcoughs&W, '&CTake... to Warriors Guild...&n'
wait 3s
wecho The poor dwarf coughs and sputters, his strength leaving his body.
wait 3s
wecho &Wa dying dwarf &Gchokes&W, '&CDerfred Axelrod... will know what to do...&n'
wait 4s
wecho The dwarf's eyes roll back in his head and he dies.
wpurge dying
wait 2s
wload obj 7568
wecho The parchment falls to the ground.
end
~
#7565
m7564 Mardif Song Trigger~
0 m 1
~
set num %random.2%
if (%num% == 1)
emote smiles warmly.
wait 3s
say Thank you kindly, friend.
wait 3s
say A song for our guests!
wait 3s
mechoaround mardif The crowd murmurs quietly, anticipating the bard's song.
wait 4s
emote clears his throat, tuning up his fiddle.
wait 3s
say An old favorite of mine, "The Cat and the Fiddle!"
wait 4s
emote &Gsings&W, '&CThere is an inn, a merry old inn, beneath an old grey hill,&n'
wait 3s
emote &Gsings&W, '&CAnd there they brew a beer so brown,&n'
wait 2s
emote &Gsings&W, '&CThat Solinari himself came down,&n'
wait 2s
emote &Gsings&W, '&COne night to drink his fill.&n'
wait 3s
emote plays a short interlude on his fiddle as some of the dwarves nearby chuckle politely.
wait 4s
emote &Gsings&W, '&CThe Barman has a tipsy cat who plays a five-string fiddle,&n'
wait 3s
emote &Gsings&W, '&CAnd up and down he runs his bow,&n'
wait 2s
emote &Gsings&W, '&CNow squeaking high now purring low,&n'
wait 2s
emote &Gsings&W, '&CNow sawing in the middle!&n'
wait 2s
mechoaround mardif A nearby drunk repeats, 'Sawing in the middle!&n'
wait 2s
emote plays another short interlude as the drunk is quickly silenced by his fellows.
wait 5s
emote &Gsings&W, '&CNow Solinari was drinking deep, and the cat began to wail.&n'
wait 3s
emote &Gsings&W, '&CA dish and the spoon on the table danced,&n'
wait 2s
emote &Gsings&W, '&CThe cow in the garden madly pranced,&n'
wait 2s
emote &Gsings&W, '&CAnd a little dog chased his tail!&n'
wait 3s
emote &Gsings&W, '&CThen Solinari took another mug, and rolled beneath his chair.&n'
wait 3s
emote &Gsings&W, '&CAnd there he dozed and dreamed of ale,&n'
wait 2s
emote &Gsings&W, '&CBut in the sky the stars were pale,&n'
wait 2s
emote &Gsings&W, '&CAnd dawn was in the air!&n'
wait 2s
emote plays more animatedly upon his fiddle as the song picks up in tempo.
wait 3s
emote &Gsings&W, '&CThe cat and the fiddle played hey diddle diddle,&n'
wait 1s
emote &Gsings&W, '&CA jig that would waken the dead!&n'
wait 2s
emote &Gsings&W, '&CHe squeaked and sawed and quickened the tune,&n'
wait 2s
emote &Gsings&W, '&CWhile the landlord shook the man in the moon,&n'
wait 1s
say "It's after 3!" he said!
wait 3s
emote saws wildly at the fiddle, the tune reaching it's high point.
wait 3s
emote &Gsings&W, '&CWith a ping and a pong, the fiddlestring broke,&n'
wait 1s
emote &Gsings&W, '&CThe cow jumped over the moon!&n'
wait 2s
emote &Gsings&W, '&CAnd the little dog laughed to see such fun,&n'
wait 2s
emote &Gsings&W, '&CAnd Suddenly the dish went off in a run with a silver sunday spoon!&n'
wait 3s
mechoaround mardif The crowd cheers wildly, applauding and standing on their chairs.
wait 3s
emote takes a deep bow, smiles at you, and tunes his fiddle once more.
wait 5s
emote Resumes playing some cheerful tunes, awaiting more requests from the floor.
elseif (%num% == 2)
emote smiles widely.
wait 3s
say Thank you, thank you very much friend!
wait 3s
say I'll sing you a fine tune...
wait 3s
emote ponders over something a moment, then nods as if decided.
wait 4s
emote clears his throat, turning his attention to you.
wait 3s
say This song is a local favorite, penned by the legendary bard Quivalen Sath...
wait 2s
mechoaround mardif The crowd falls silent in anticipation.
wait 4s
say The Dwarven Marching Song!
wait 2s
mechoaround mardif The crowd erupts in wild cheering!
wait 3s
emote waits for the cheering to die down, and then starts into his tune.
wait 4s
emote starts to play, singing solemnly in a deep baritone.
wait 3s
emote &Gsings&W, '&CUnder the hills, the heart of the axe,&n'
wait 3s
emote &Gsings&W, '&CArises from cinders, the still core of the fire.&n'
wait 3s
emote &Gsings&W, '&CHeated and hammered, the handle an afterthought,&n'
wait 3s
emote &Gsings&W, '&CFor the hills are forging the first breath of war.&n'
wait 3s
emote &Gsings&W, '&CThe soldier's heart sires and brothers,&n'
wait 2s
emote &Gsings&W, '&CThe battlefield.&n'
wait 3s
emote &Gsings&W, '&CCome back in glory... Or on your shield.&n'
wait 3s
emote plays skillfully upon his fiddle, some of the dwarves beginning to stomp their feet.
wait 4s
mechoaround mardif A few of the dwarves join in on the next chorus, singing close harmony.
wait 3s
emote &Gsings&W, '&COut of the mountains in the midst of the air,&n'
wait 3s
emote &Gsings&W, '&CThe axes are dreaming, dreaming of rock.&n'
wait 3s
emote &Gsings&W, '&COf metal alive through the ages of ore,&n'
wait 3s
emote &Gsings&W, '&CStone on metal, metal on stone.&n'
wait 3s
emote &Gsings&W, '&CThe soldier's heart contains and dreams...&n'
wait 2s
emote &Gsings&W, '&CThe battlefield.&n'
wait 4s
emote &Gsings&W, '&CCome back in glory,&n'
wait 3s
emote allows the dwarves of the tavern to sing the refrain.
wait 3s
mechoaround mardif The dwarves sing, 'Or on your shield.'
wait 4s
mechoaround mardif The Beer Hall is now alive with singing and stomping as the bard goes into the end of his tune.
wait 5s
emote &Gsings&W, '&CRed of iron imagined from the vein,&n'
wait 3s
emote &Gsings&W, '&CGreen of brass, green of copper.&n'
wait 3s
emote &Gsings&W, '&CSparked in the fire, the forge of the world,&n'
wait 3s
emote &Gsings&W, '&CConsuming in its dream as it dives into bone.&n'
wait 3s
emote &Gsings&W, '&CThe soldier's heart lies down, completes...&n'
wait 3s
mechoaround mardif The note hangs in the air for a long moment, and then storms back in, the dwarves all singing in harmony.
wait 5s
mechoaround mardif The dwarves sing, 'The battlefield.'
wait 3s
mechoaround mardif The dwarves sing, 'Come back in glory,'
wait 3s
emote plays the last notes of the song and finishes the tune solo.
wait 4s
emote &Gsings&W, '&COr on your shield.&n'
wait 2s
mechoaround mardif The Beer Hall falls silent for a moment.
wait 3s
mechoaround mardif Then, there is an eruption of wild hooting and cheering.
wait 2s
emote takes a deep bow.
wait 4s
emote waits until the applause dies away, then takes a swig of his ale, awaiting further requests.
end
~
#7566
m7566 Alarum Trigger~
0 j 100
~
if (%object.vnum% == 7568) then
wait 1s
blink %actor%
wait 2s
emote looks at the note, eyes widening.  His mouth opens and closes a few times.
wait 3s
emote sets his jaw firmly, his plans forming in his mind.
wait 4s
emote turns to the other dwarves in the guildhall.
wait 1s
say Boys.
wait 2s
emote nods grimly.
wait 2s
say We've got work to do.
wait 3s
mechoaround Derfred The other dwarves look up from their card games, a little surprised.
wait 3s
mechoaround derfred A few dwarves grumbles, 'Can it be?  After all these years?'
wait 2s
nod dwarven
wait 2s
say Ring the bell!  Grab yer axes and git down to the mines.  Now!  Move it!
wait 3s
mechoaround derfred One of the dwarves pulls a chain, causing the long-disused alarum bell to ring!
wait 2s
mechoaround derfred The dwarves hastily get up from their tables, arm up, and charge out the door, leaving you alone with the guildmaster.
mpurge
mforce %actor% zzzz
wait 3s
mechoaround derfred In the strange silence of the empty guildhall, Defred Axelrod turns to you.
wait 3s
say You've done well, friends.  Take this.
wait 2s
emote drops something on a table.
wait 1s
mforce %actor% get pass
wait 2s
say Use that to git yourself into the mines.  Help us if ye can.
wait 3s
say But, if ye don't hurry, there'll be nothin' left for ye to thrash, so hurry up!
wait 3s
emote unlocks the weapons cabinet, preparing to arm the dwarves that come in to help protect the community.
wait 3s
emote turns to look at you.
wait 2s
emote &Gbarks&W, '&CWell, what are ye waitin' for!  GO!&n'
end
~
#7567
m7567 Warl Nog Puts Things Away Trigger!~
0 j 100
~
if (%object.vnum% == 7592) then
wait 1s
nod %actor%
wait 2s
say Good, good.  Bring 'em back here when ya find 'em.
wait 3s
emote looks over the coveralls carefully.
wait 4s
frown
wait 2s
say How did these get so filthy!  PAH!
wait 2
grumble self
wait 1
open cabinet
wait 1s
emote Carefully hangs up the coveralls and places a little tag on them that says '&WDirty&n'.
wait 2s
put coveralls cabinet
wait 1s
close cabinet
wait 2s
emote dusts off his hands and returns to his work in a huff.
elseif (%object.vnum% == 7593) then
wait 1s
nod %actor%
wait 2s
say Yes, yes.  Good.  Lets have a look at that.
wait 2s
emote turns the pickaxe over carefully in his hands, examining it.
wait 4s
say Fine.  It's in fine shape.  Good work.
wait 2s
emote puts the pickaxe neatly away upon the rack.
wait 1s
put pickaxe 2.rack
wait 2s
emote goes back to work in a huff.
elseif (%object.vnum% == 7583) then
wait 1s
emote takes the white coveralls from you, looking them over.
wait 3s
say Excellent.  No rips or tears.
wait 2s
open cabinet
wait 1s
emote places the coveralls neatly on a hanger and puts them away.
wait 1s
put coveralls cabinet
wait 1s
close cabinet
wait 1s
emote dusts off his hands, returning to work with a stoic grunt.
else
wait 1s
blink %actor%
wait 2s
say What's this?  Bah.  I'm not here to put away your junk.
wait 2s
frown %actor%
wait 1s
open door
wait 1s
say It goes out HERE!
wait 1s
northwest
wait 2s
emote hefts some refuse into the refuse well.
wait 1s
drop all
wait 2s
emote dusts off his hands and congratulates himself on a job well-done.
wait 2s
southeast
wait 2s
emote returns to work with a sigh.
end
~
#7568
r7568 Don't Get Trapped Trigger~
2 c 100
west~
if (%actor.vnum% == -1)
wait 1s
wsend %actor% You squeeze through the narrow crack in the rock and out of the small room.
wechoaround %actor% %actor.alias% squeezes through the narrow crack in the rock and out of the small room.
wteleport %actor% 7567
end
~
#7569
o7569 Coin Toss Trigger~
1 c 111
flip~
if (%arg% != coin) then
osend %actor% What would you like to flip?
else
osend %actor% You handily flip the coin into the air.
oechoaround %actor% %actor.alias% flips a single gold coin into the air.
wait 2s
osend %actor% You snatch the coin out of the air and look at it.
oechoaround %actor% %actor.alias% snatches the coin out of the air.
wait 3s
eval coin %random.2%
if (%coin% == 1)
oecho The 'Heads' side of the coin, the woman, comes face-up.
elseif (%coin% == 2)
oecho The 'Tails' side of the coin, the raven, comes face-up.
end
end
~
#7570
r7569 ZZZZ Trigger~
2 c 100
zzzz~
if (%actor.vnum% == -1) then
wload obj 7567
wdoor 7567 east flags a
wdoor 7567 east name crack
wdoor 7567 east room 7568
wdoor 7568 west flags a
wdoor 7568 west name crack
wdoor 7568 west room 7567
wait 150s
wdoor 7567 east purge
wdoor 7568 west purge
end
~
#7571
r7571 and r7577 Zpider Purge Trigger~
2 b 100
~
set num %random.3%
if (%num% == 1)
wecho Something vanishes just out of the corner of your eye.
wpurge
wload obj 7572
wait 20s
elseif (%num% == 2)
wecho You hear faint chittering.
wait 8s
wecho The chittering dies away.
wait 10s
elseif (%num% == 3)
wecho A tiny spider crawls into a crack in the wall and is gone.
wpurge
wload obj 7572
wait 20s
end
~
#7572
o7575 Spider Control~
1 b 100
~
   set num %random.3%
   if (%num% == 1)
oecho Spiders crawl in and out of the cracks in the walls!
oload mob 7587
oload mob 7587
opurge zpider
wait 20s
elseif (%num% == 2)
oecho You hear loud chittering and clicking!
wait 8s
oecho The chittering becomes slightly quieter.
oload mob 7587
opurge zpider
wait 10s
elseif (%num% == 3)
oecho You hear a loud hiss nearby!
oload mob 7587
opurge zpider
wait 20s
end
~
#7573
r7572 Burninate the Spiders!~
2 c 100
turn~
if (%actor.vnum% == -1) then
if (%arg% != knob) then
wsend %actor% What do you wish to turn?
else
set num %random.3%
if (%num% == 1)
wsend %actor% You turn the knob on the lantern.
wechoaround %actor% %actor.alias% turns the knob on the lantern.
wait 2s
wecho Nothing seems to happen!
elseif (%num% == 2)
wsend %actor% You turn the knob on the lantern.
wechoaround %actor% %actor.alias% turns the knob on the lantern.
wait 2s
wecho Spiders emerge from the nest and attack!
wload mob 7587
wait 1s
wforce zpider kill %actor%
wait 1s
elseif (%num% == 3)
wsend %actor% You turn the knob on the lantern.
wechoaround %actor% %actor.alias% turns the knob on the lantern.
wait 2s
wecho &YThe lantern ignites!&n
wait 2s
wecho A spider crawls up the web toward the lantern just as one of the webs catches fire.
wait 3s
wecho &RThe webs are soon ablaze, the spiders trapped within shrieking and clicking as they burn!&n
wait 1s
wpurge
wload mob 7598
wait 1s
wecho &KFrom the smoke created by the burning webs, seemingly from the darkness itself, a single, huge spider lunges at you!&n
wait 2s
wforce queen k %actor%
end
end
end
~
#7574
r7565 End Dwarf Purge Trigger~
2 c 100
zxc~
if (%actor.vnum% == -1) then
wait 2s
wpurge zilgar
end
~
#7575
m7575 Kill the Oozes Trigger!~
0 g 100
~
if (%actor.vnum% == -1)
wait 1s
kill yyyy
end
~
#7578
o7578 Horn of Blasting Trigger~
1 c 100
use~
say My trigger commandlist is not complete!
~
#7580
r7580 TRIGGER1 Trigger~
2 c 100
trigger1~
if (%actor.vnum% == -1)
wpurge
wload obj 7574
wload mob 7568
end
~
#7581
r7580 TRIGGER2 Trigger~
2 c 100
trigger2~
if (%actor.vnum% == -1)
wpurge
wload obj 7574
wecho Breslin Axehand shuffles off to the northeast.
end
~
#7582
m7582 No Go Right Path Yet Trigger~
0 g 100
~
if (%direction% == northeast) and (%actor.vnum% == -1) then
wait 1s
emote glares at you as you approach.
wait 2s
say So, ye've come to do some explorin', have ye?
wait 3s
snort
wait 1s
say Well, ye can go down, I ain't gonna stop ya.  But stick to the left-hand path.
wait 3s
say Right-hand path is for authorized dwarves *ONLY*.  So, less ye've got a deep pass, I ain't gonna let you through.
wait 4s
emote adjusts his eyepatch casually, watching you very, very closely.
end
~
#7583
m7582 Deep Pass Trigger~
0 j 100
~
if (%object.vnum% == 7567) then
wait 1s
emote almost chokes as you hand him the deep pass.
wait 2s
emote looks it over quickly, then back up at you.
wait 3s
say This... this be grave indeed!  Quick now, git in there!
wait 2s
drop pass
wait 1s
mforce %actor% get pass
wait 1s
msend %actor% The dwarf unlocks the right-hand passage, ushering you through hurriedly.
mechoaround %actor% The dwarf unlocks the right-hand passage, ushering %actor.alias% through hurriedly.
wait 3s
say Git in there and give 'em hell!
wait 2s
msend %actor% You descend into the deep tunnels.
wait 1s
mteleport %actor% 7596
msend %actor% You hear the gate close and lock behind you.
wait 1s
mforce %actor% look
say Sorry lads...
wait 2s
emote dusts off his hands.
wait 2s
say Deep pass is good fer one body only.  He'll have ta let ye in some other way.
wait 3s
shrug self
wait 2s
emote stands there, seemingly oblivious to the frustrating time he has chosen to go 'by the book'.
end
~
#7586
m7586 Victory!~
0 c 100
qwerty~
if (%actor.vnum% == -1) then
wait 2s
emote approaches cautiously, a few of his fellows a few steps behind.
wait 3s
emote widens his eyes upon sighting the body at your feet.
wait 2s
say Reorx be praised, ye've done it lad!
wait 3s
mechoaround Zilgar Scattered cheers echo through the caverns, newly cleared of the menace!
wait 3s
say We'll spread word of yer deed.  Bring that claw...
wait 2s
emote pauses a moment, glancing again at the fallen demon's corpse.
wait 3s
say Bring that claw there with ye.
wait 2s
say Helgrin'll reward ye handsomely for it I wager.  Or I suppose ye can keep it, if that's the sort of thing ye like.
wait 4s
frown self
wait 2s
emote turns at that, and dashes off out of the corridor.
mforce %actor% zxc
end
~
#7587
o7575 Hatchling Load/Purge Trigger~
1 b 33
~
set num %random.3%
if (%num% == 1)
oload mob 7587
oechoaround zpider Spiders crawl all over and around the tangle of cobwebs.
wait 160s
elseif (%num% == 2)
oload mob 7587
oechoaround zpider Something larger stirs behind the cobwebs!
wait 5s
oechoaround zpider Some of the smaller spiders disappear into the webs as though preparing for something.
wait 10s
elseif (%num% == 3)
oload mob 7587
oechoaround zpider You feel something crawling up your leg!
wait 160s
end
~
#7588
m7584 Death Load Trigger~
0 f 100
~
eval objload %random.14%
if (%objload% == 1) then
mload obj 7582
elseif (%objload% == 2) then
mload obj 7584
elseif (%objload% == 3) then
mload obj 7593
else
return 0
end
~
#7590
m7599 Kharasz Death Trigger~
0 f 100
~
mload obj 7599
mload mob 7586
mforce %actor% qwerty
mechoaround kharasz The hideous Mold Demon dies with a blood-curdling roar!!
return 0
end
~
#7591
m7590 Cydratha Greeting Trigger~
0 g 100
~
if (%direction% == east) and (%actor.vnum% == -1) then
wait 1s
mechoaround cydratha A wicked and malicious grin slowly spreads across the woman's face.
wait 3s
mechoaround cydratha She does not move from her reclining position upon the rock wall.
wait 3s
say So... you've come, just as planned.  Driven by your curiosity...
wait 2s
emote traces a single finger over one of her rune-painted breasts.
wait 3s
say It is so nice to have some lively prey... apart from these tiresome dwarves.
wait 3s
emote slowly moves her eerie gaze to a pile of dried bones upon the ground.
wait 2s
grin self
wait 1s
emote looks back up at you, licking her crimson-coloured lips.
wait 2s
say You lack a healthy sense of self-preservation, to have come this far.
wait 3s
emote laughs huskily.
wait 2s
say Even if you manage to destroy me...
wait 2s
emote looks you over, her lips turning up in a mocking smirk.
wait 2s
say Which I doubt you will...
wait 3s
say The summoning is complete.  Soon the scourge of the Black Wind shall be unleashed upon all of the undermountain.
wait 4s
say You will not stop us!
wait 1s
cackle
wait 3s
mechoaround cydratha The smirk fades with the laughter, and she fixes her eyes upon you.
wait 3s
say But go ahead and try.  I welcome the chance for a new game.
wait 2s
mechoaround cydratha With that, she lounges back, yawning.  She makes no move to stop you in your course at the moment.
end
~
#7592
m7591, m7592, m7593, and m7594 Death Fission Trigger~
0 f 100
~
mechoaround yyyy Just as you slay the creature, it splits into two smaller slimes!
mload mob 7584
mload mob 7584
return 0
end
~
#7594
m7594 Wubba Wubba~
0 b 25
~
set num %random.3%
if (%num% == 1)
wait 1s
mechoaround cube Wubba Wubba Wubba.
wait 4s
mecho cube The air grows very thin for a moment.
wait 3s
mechoaround cube Wubba Wubba Wubba Wubba.
wait 3s
mecho cube A gelatinous cube slowly approaches.
elseif (%num% == 2)
wait 1s
mecho cube You hear a strange sloshing sound nearby.
wait 4s
mechoaround cube Wubba Wubba.
wait 3s
mecho cube Something moves in the darkness.
elseif (%num% == 3)
wait 1s
mecho cube Suddenly, it is too quiet.
wait 4s
mecho cube Something stirs in the darkness.
wait 3s
mechoaround cube Wubba Wubba Wubba.
end
~
#7595
m7595 death load m7596~
0 f 100
~
mecho monster As you destroy the creature, a dwarf slowly crawls out of hiding.
mload mob 7596
end
~
#7596
m7596 Dying Dwarf~
0 c 100
look~
wait 1s
mforce %actor% xyz
wait 36s
end
~
#7597
m7597 Poison Mist Load!~
0 l 30
~
if (%random.5% == 1) then
emote utters the words, 'gvathaxthm vffthra'.
emote conjures a poisonous mist!
mload mob 7597
wait 26s
end
~
#7598
o7599 !DROP Trigger~
1 h 100
~
osend %actor% The cursed claw clings to your hand.  You cannot drop the unholy thing!
return 0
end
~
#7599
NOT USED~
1 c 1
blast~
if (%actor.pos% == fighting) then
oecho %arg%
set num %random.10%
 if (%num% == 1) then
   elseif [(%num% == 2) and (%arg.level% >= 20)] then
     osend %actor% A dull ringing emits from Nzaalhir!
     oechoaround %actor% A dull ringing from %actor.alias%'s hammer hurts your ears!
     oforce %arg% emote winces in pain!
   elseif (%num% == 3) then
     osend %arg% A bright blue spark emits from Nzaalhir, shocking you!
     oechoaround %arg% A spark of electricity from Nzaalhir shocks %arg.alias%!
     odamage %arg% 15
   elseif (%num% == 4) then
     osend %arg% A huge bolt of lightning arcs from Nzaalhir, striking you!
     oechoaround %arg% A bolt of lightning arcs from Nzaalhir, striking %arg.alias%!
     odamage %arg% 15
   elseif (%num% == 5) then
     osend %actor% A crack of thunder emits from the hammer, shaking you to the core!
     oechoaround %actor% A crack of thunder shakes the ground itself!
     odamage %actor% 20
   else
     oecho Sparks fly out from Nzaalhir.
 end
end
~
$~
