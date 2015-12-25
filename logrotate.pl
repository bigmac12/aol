#!/usr/bin/perl -w
# This rotates the logs. Woo hoo!
# --GAN, 9-27-2000

# not done!
exit 0;

# Declarations
$log_dir = "log/";
$rotations = 3;
$rot_index = 1;

$raw_file_list = `/bin/ls -1 $log_dir`;
@file_list = split(' ', $raw_file_list);
@files = grep /\.log/, @file_list;
while($rot_index < ($rotations + 1))
{
   @files = grep !/\.$rot_index/, @files;
   $rot_index++;
}
foreach $curr (@files)
{
   unlink(sprintf("%s%s.%s", "$log_dir", "$curr", "$rotations"));
   $rot_index = 1;
   $curr_index = $rotations;
   while($curr_index > $rot_index)
   {
      $nf = sprintf("%s%s.%s", "$log_dir", "$curr", "$curr_index");
      $curr_index--;
      $of = sprintf("%s%s.%s", "$log_dir", "$curr", "$curr_index");
      rename $of, $nf;
   }
   print "$curr\n";
}
exit 0;
