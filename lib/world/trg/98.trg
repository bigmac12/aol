#9800
(%has.x%)Test~
2 c 100
fish~
   if (%actor.vnum% == -1) && (%has.rod%)
      if (%arg != rod)
         wsend %actor% Nope
      else
            wsend %actor% has.x variable is working
   else
   wsend %actor% has.x variable is working
end
~
#9828
m9828 Inv Purge~
0 e 100
sells~
say Please excuse me a moment.
wait 4
emote carries his new item upstairs to add to his collection.
up
drop all
down
wait 6
~
#9831
r9831 Gnome talking to Mish Priestess~
2 g 100
~
wait 1s
wecho You walk in and immediately see a nearly hysterical male gnome talking to a young woman wearing sky-blue robes. 
wait 6s
wecho The hysterical gnome says, &COhbyReorxIswearthatIdidn'tmeantodothat!Ineverimaginedthatitwouldcrashthroughawallanduncovermovingdead!WhichareveryinterestingImightadd.&n'
wait 10s
wecho The young priestess gets down onto her knees and puts her hand on the gnome's shoulder, trying to comfort him.
wait 8s
wecho The young priestess keeps her hand on his shoulder and speaks, &CI'm sorry, sir gnome, but I don't understand.  Please talk slower.  What's wrong?  Are you okay?&n'
wait 10s
wecho The gnome continues, &CYouseemycommittee,the"CommitteeInChargeOfThePossibilityOfUsingCowsToTravelBackwardsThroughTime"ranintoadeadendandwemadethisdevicetohelpworkonourproblem!Butthenduringthetrialrun,myCrabFinderBoilerandSeagullChaserAwayerwenthaywireandcrashedthrougharockwall!&n'
wait 14s
wecho The priestess stares at the gnome.  There is an obvious look of confusion on her face, but it's difficult to tell if her confusion stems from the name of his committee or if it's simply because he speaks too fast.
wait 12s
wecho The priestess stands up and sighs, then calls over a man dressed in the clothing of a monk of Majere.  &CLarim?  Could you take my place?  I think I hear somebody calling for a healer...&n'
wait 13s
wecho The monk walks over and the priestess rushes away.  The gnome continues his hysterical, even for gnome standards, rate of speech while the monk listens to the gnome in silence. 
~
#9878
r9878 Random pegleg zombie ~
2 bg 30
~
if (%actor.vnum% == -1) then
wait 2s
wecho A zombie pirate with a pegleg hobbles in from the north.
wait 4s
wecho It wanders around, first getting fairly close to you, then it turns and hobbles towards the cavern wall to the south.
wait 6s
wecho Right before reaching the rocky wall, the zombie steps on a skull with his pegleg and the skull shatters.
wait 5s
wecho The zombie falls over, his face slamming into the cavern wall and snapping his head back onto his back at an impossible angle.  While falling over his pegleg gets stuck in the skull and snaps off of his leg.
wait 1s
wload obj 9887
wait 10s
wload mob 9891
wecho The zombie lays still for a moment, then begins to crawl off into the tunnel to the east.
wforce onelegged east
end
else
end
~
#9899
r9872 Wave~
2 h 100
~
wait 1
wecho  A wave appears, washing debris out to sea.
~
$~
