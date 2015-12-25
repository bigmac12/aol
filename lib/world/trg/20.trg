#2000
Trig Test~
2 d 100
BANG~
wecho A huge toy gun magically appears and a flag comes out with the word BANG! on it.
~
#2009
m2065 Trainer Greet~
0 g 100
~
wait 3s
emote turns to look your direction, fixing you with a crystalline gaze.
wait 3s
say If you're with the Guild, you can inquire with me about disguises and weapon venom. If you're not, I suggest you leave as I have nothing to offer you.
wait 4s
emote turns away, assisting a fellow rogue with something.
~
#2010
m2010 RogueTrainer Greet~
0 g 100
~
wait 3s
emote turns in your direction, adjusting his spectacles to have a look at you.
wait 3s
say Hello! If you're with the Guild you can ask me about lock picking, or about forgery. I can tell you a bit about that, as well as train you how to do it yourself.
wait 5s
emote adjusts his spectacles again, looking more closely at you.
wait 3s
say If you're not with the Guild, get lost.
wait 3s
emote returns to supervising his students, muttering to himself.
~
#2011
m2010 RogueTrainer Pick/Forge~
0 d 1
Pick Locks Forgery~
if (%speech% /= pick locks) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
msend %actor% Tarnael pushes his spectacles up, examining you closely.
mechoaround %actor% Tarnael pushes his spectacles up, examining %actor.alias% closely.
wait 3s
say You aren't with the Guild! Get outta here before I shove this pick up your ass.
wait 2s
grumble
wait 3s
emote returns to supervising his students.
else
wait 3s
say Hmm? Oh, as yes... yes I can teach you about picking locks, but in our business we call it the Black Art.
wait 3s
say Step over here, and we'll get started. I'm going to tell you a bit about the process, and the background before we start.
wait 4s
say We gentleman are reknown in our business for being able to get inside places designed to keep people out, whether it be a Lord's mansion, a treasure chest, or anything else with a lock on it.
wait 2s
chuckle
wait 4s
say The black art is one of the defining skills among our profession, so learn it well.
wait 3s
say In order to open a lock, you will need only one tool. Rather one set of tools, and that is a set of lock picks. In our business they're known as charms, or dubs.
wait 5s
say They're small, they're light, they're easily hidden. This makes them perfect for our use, and they'll become your best friend in your career. By the way, a gentleman who makes his living by the black art is known as a Gilt, or a Rum Dubber.
wait 6s
grin
wait 3s
say You could use a Betty, that's a crowbar, but those are for thugs and footpads. Real thieves use charms, shows some integrity and skill.
wait 3s
say Now. If you look at this lock here, you just need to select..
wait 4s
emote goes into great detail on the usage of lockpicks and the opening of locks, ensuring that you learn at least the basics of the 'Black Art'.
wait 3s
mforce %actor% prac pick locks
end
elseif (%speech% /= forgery) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You are not in the Guild. I suggest you leave before you find yourself wanted in Solamnia on fabricated charges.
wait 2s
smirk
else
wait 3s
say Alright, I'll tell you alittle about forgery. I'll warn you though, it's not exactly something I can teach.
wait 3s
say The art of forgery is very difficult, and only the best can do it well. It is the art of making fake documents look like the real thing. An example would be a warrant for arrest.
wait 5s
say A successful forger would be able to make a document that could fool any Knight or guard who viewed it. There are many details in forging documents, from penmanship to noble seals.
wait 5s
say I offer a few items for forgery here, at a small fee. As I said before, I'm afraid I can't really train you in forgery though. It is too complicated a skill to be taught.
end
end
~
#2064
m2064 Combat Trainer~
0 d 1
kick trip dodge jawstrike throw~
if (%speech% /= kick) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 2s
say You ain't in the Guild, so I ain't teaching you squat. Now beat it before I kick yer ass.
else
wait 2s
say Kick. Right... Yer serious? Come on, mate. Kick? Just bloody kick yer opponent, it ain't brainwork here.
wait 2s
mforce %actor% prac kick
end
elseif (%speech% /= trip) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 2s
say You ain't in the Guild, so I ain't teaching you squat. Now beat it before I call the boys ta break yer legs.
else
wait 2s
say Trip.. Aye, I'll teach ya about trip.
wait 2s
grin
wait 1s
msend %actor% Draynen quickly sweeps his leg out, tripping you!
mechoaround %actor% Draynen quickly sweers his leg out, tripping %actor.alias%!
mforce %actor% sit
wait 2s
emote laughs boisterously.
wait 2s
msend %actor% Draynen offers you a hand.
mechoaround %actor% Draynen offers %actor.alias% a hand.
wait 1s
mforce %actor% stand
wait 3s
say Alright.. now that I've had me fun, and ye've seen the skill in use, let's practice it a bit.
wait 2s
mforce %actor% prac trip
end
elseif (%speech% /= dodge) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You ain't in the Guild, so I ain't teaching you squat. Now beat it before I call the boys ta break yer legs.
else
wait 3s
say So, you want me to teach you to dodge, huh? Alright then.
wait 4s
say DODGE!
msend %actor% Draynen punches you dead in the face! &rOUCH&n!
mechoaround %actor% Draynen punches %actor.alias% dead in the face!
wait 2s
emote chuckles, wincing at the hit.
wait 4s
say Sorry about that mate, you gotta be quicker than that if ye wanna dodge.
wait 3s
say Alright lets practice it a bit, I'll go easier on ya..
wait 1s
grin
wait 3s
mforce %actor% prac dodge
end
elseif (%speech% /= jawstrike) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You ain't in the Guild, so I ain't teaching you squat. Get outta here, before I show it to ya firsthand!
else
wait 3s
think
wait 2s
say Jawstrike, hmm? This is a useful skill mate, so listen up..
wait 3s
say Say you got a mage givin ya trouble, or a cleric for that matter. It'd be real useful ta shut em up, keep em from castin them spells, aye?
wait 5s
say Well, that's what yer tryin to do with this skill. Ye just punch at em, hit their jaw right here..
wait 2s
msend %actor% Draynen places his fist against your jaw at a very specfic place and applies a small amount of pressure. It hurts, but not too badly.
mechoaround %actor% Draynen places his fist against %actor.alias%'s jaw, pushing slightly.
wait 5s
say Ya do that, except ya slam yer fist into their jaw hard as ye can. Oughta shut em up real quick. Useful for normal folk too.. someone won't shut up, just slap em round alittle.
wait 5s
grin
wait 2s
say Now, let's practice it a bit.. dun go sluggin me in the mouth though, or i'll beat ya.
wait 3s
mforce %actor% prac jawstrike
end
elseif (%speech% /= throw) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say Throw.. ye wanna learn about throw, huh? Well.. it's yer lucky day.
wait 2s
msend %actor% Draynen grabs you, hoisting you off of the ground.
mechoaround %actor% Draynen grabs %actor.alias%, hoisting them off of the ground.
wait 2s
say Yer first lesson..
wait 2s
grin
wait 3s
msend %actor% Draynen moves towards the door, pitching you out of it!
mechoaround %actor% Draynen moves towards the door, pitching %actor.alias% out of it!
mforce %actor% n
wait 2s
emote dusts his hands off, returning to the training area.
wait 2s
say Anybody else!? No? GREAT.
else
wait 3s
say Throw? Yea.. I can tell ya about that. It ain't that hard really, who hasn't thrown somethin before?
wait 2s
chuckle
wait 4s
say But, we're talkin about throwin something at somebody with accuracy, and with force. Little bit of skill in that I suppose. So..
wait 4s
say You can't throw just anythin.. it's gotta be light enough for you to pick up an' carry of course. Best things are throwin' knives, throwin' stars, stuff like that.
wait 6s
say Of course, just about anythin you can pick up'll do in a pinch. Heavier it is, the more hurtin ye'll put on em. Best spot ta aim for is the head..
wait 2s
grin
wait 3s
say Ye can practice on that dummy over there. Dun go throwin anythin at anybody else though, or you'll get beat.
wait 3s
mforce %actor% prac throw
end
end
~
#2065
m2064 RogueCombat 2~
0 d 1
dirtkick backstab knockout~
if (%speech% /= dirtkick) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You ain't in the Guild, so I ain't teaching you squat. Now beat it before I call the boys ta break yer legs.
else
wait 3s
say Right, dirtkick.. yea, I can tell you alittle about that.
wait 3s
say We ain't got any dirt in here, so I can't show ya properly, but I'm gonna show ye the movement. Now, watch my foot..
wait 4s
emote sweeps his foot across the training mat, lifting it at the last moment.
wait 3s
say Ye can use this a couple ways, another one of them skills that works well fer different situations.
wait 3s
say Say yer in a sticky spot, some pig's got ye cornered and ye don't wanna scuffle with him.. well, ye use this skill and blind him, then make yer getaway.
wait 5s
say Another way.. say ye got some runt mage, or some mark where you want em, but they might give ya some trouble.. kick some dirt in their face!
wait 2s
chuckle
wait 2s
say It'll blind em, then ye can do as ye please fer a short time. Keep in mind, ye can't go kickin dirt in people's faces if there ain't no dirt. Usually gotta be outside fer this skill to be of any use.
wait 5s
say Go ahead and practice as ye can..
wait 3s
mforce %actor% prac dirtkick
end
elseif (%speech% /= backstab) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You ain't in the Guild, so I ain't teaching you squat. Now beat it before I call the boys ta break yer legs.
else
wait 3s
say Ahh.. yes. This skill is more technical than the others, so pay attention!
wait 3s
emote calls Jilena over, turning her so her back is visible to you.
wait 2s
say Alright, watch the points I show you.
wait 3s
emote jabs his forefinger into several different spots along Jilena's shoulders and back, each spot making her wince in pain.
wait 3s
say There are weak spots on everybody, mostly in the same places. These spots, if ye hit em.. they'll drop yer opponent right quick. Some of em are near fatal.
wait 5s
say There're two keys to usin this skill well. The first, hittin the right spots. The second, surprise. As with everythin we do, surprise and stealth is key. If yer target is alert, it makes it mighty hard to get in a surprise jab.
wait 6s
say Go ahead and practice on Jilena for a bit, with yer finger only! Ye go jabbin 'er with a dagger and I'll do some jabbin of my own, of a more permanent kind.
wait 2s
emote waits to make sure his point is made clear then steps back, allowing you to practice.
wait 3s
mforce %actor% prac backstab
end
elseif (%speech% /= knockout) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You ain't in the Guild! Get outta here before ye experience it firsthand.
else
wait 3s
say Ahh.. ye wanna learn about knockin an opponent out, eh? Very useful skill.
wait 3s
say This is one of those technical skills, so you'll need to pay attention.
wait 2s
say There'll be times when ye don't wanna tuck a mark inta bed, but ye don't want em goin and makin a fuss while ye work, right? Or perhaps ye just need ta get by em, but they just aren't co-operatin.
wait 6s
say Well, this is the skill ye wanna use. It works a bit like the old shoulder tap, ye should have the element of surprise on yer side.
wait 3s
emote calls Jilena over, turning her so her back is to you.
wait 3s
say What ye do is strike em real quick like, right along the collar-bone or side of the neck.. right here.
wait 3s
emote points to the locations along Jilena's neck and collarbone, illustrating the lesson.
wait 3s
say Now, ye need to use a blunt object.. the hilt or pommel of most weapons'll do, most times. Some gentleman prefer ta use a sap, one of them pouches filled with lead. Does the trick well enough.
wait 6s
say Now, if ye done it correctly, yer mark should go limp as a fish and fall on the ground unconcious. Once ye've done that, yer workin on a time table kid. They don't stay out fer long, usually.
wait 6s
say Take a couple minutes to familiarize yerself with the places ta strike, but dun go hittin Jilena, or ye'll be visitin dreamland yerself.
wait 3s
mforce %actor% prac knockout
end
end
~
#2066
m2064Greet RogueTrainer~
0 g 100
~
wait 3s
emote glances in your direction as you enter, assessing you.
wait 4s
say This is combat trainin fer Guild members. If yer in the Guild, ye can ask me about combat skills and I'll tell ya a little about em and train ya.
wait 5s
say If yer not in the Guild, get the hell out.
wait 3s
emote returns to supervising his students.
~
#2067
Disguisepoison~
0 d 1
disguise envenom~
if (%speech% /= disguise) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You are not in the Guild. I suggest you leave, or something bad may happen to you.
wait 2s
msend %actor% Rhanna's eyes flash dangerously, giving you the impression she means exactly what she says.
mechoaround %actor% Rhanna's eyes flash dangerously at %actor.alias%, giving the impression that she is quite serious.
else
wait 3s
nod
wait 2s
say Yes.. I can teach you how to use this skill, though it will remain your responsibility to use it well.
wait 4s
say You can disguise yourself as anyone you wish, any path of life as long as you act the part. You must make your audience believe that you are what you are pretending to be.
wait 6s
say There are a few general tools you must use.. facial putty, to sculpt the features. It won't do if you still look like yourself, simply dressed in a costume. Very few times you will not need the putty to aid in your disguise.
wait 7s
say Having a specific item recognizeable to the path of life you're attempting to mimic is a must. People won't believe you're a beggar if you're wearing expensive armor and gear.
wait 6s
say Some disguises may require a number of items in order to accurately portray the person you're trying to disguise yourself as. It is up to you to discover what you can use, and how you can use it.
wait 6s
say If you have disguised yourself accurately, you will look like any other person among the crowds. You will not be recognizeable to anyone, even those that know you personally. However, there are those that can see through the disguise, if they truly look.
wait 8s
say Mostly, these are our brethren, and pose little danger unless you have crossed them somehow. Be wary though, for certain monks have an annoying knack for seeing through a disguise, and they may alert others.
wait 6s
say I have a few items to get you started, which I charge a small fee for. You may purchase them after our lesson, if you're so inclined.
wait 3s
mforce %actor% prac disguise
end
elseif (%speech% /= envenom) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You are not of the Guild, thus I will not teach you. I suggest you leave, or you can experience it's effects firsthand.
wait 4s
msend %actor% Rhanna's eyes flash dangerously, giving you the impression she means exactly what she says.
mechoaround %actor% Rhanna's eyes flash dangerously at %actor.alias%, giving the impression that she is quite serious.
else
wait 3s
think
wait 2s
say Yes.. I can teach you about poisoning your weapons.
wait 3s
emote moves to the desk at the center of the room, taking a vial from the rack.
wait 3s
say Watch closely. Poisons can be very dangerous to yourself if applied incorrectly.
wait 3s
emote unstoppers the vial carefully, and gets a dagger from the desk.
wait 4s
emote slowly, carefully applies a thin coating of the dark liquid along the length of the blade.
wait 5s
emote places the empty vial on the desk, and raises the dagger for you to examine.
wait 4s
say Note that I have not let the poison touch my skin. If it does, I will be affected.
wait 3s
say Assuming you have applied the toxin correctly, you have a short amount of time before it becomes ineffective. Thus, once applied you will want to strike your victim quickly. It is only good for one use.
wait 5s
say Once poisoned, your victim will become weak and nauseous. It will also hamper their ability to heal wounds while affected, as well as exhaust them physically, making them slow.
wait 5s
say You may practice here, on these weapons. Use caution, please. I don't wish to waste a large amount of anti-toxin curing a clumsy footpad.
wait 4s
say Once you've finished, I have both toxin and anti-toxin available, for a small fee.
wait 3s
mforce %actor% prac envenom
end
end
~
$~
