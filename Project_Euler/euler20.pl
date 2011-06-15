use bigint;
my $factorial = 1;
$factorial *= $_ foreach 1..100;
$str = sprintf("%s",$factorial);
@stray = split //, $str;
print $str."\n";
my $sum = 0;
$sum += $_ foreach (@stray);
print $sum."\n";